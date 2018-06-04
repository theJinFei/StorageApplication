#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>

//error type
#define BIT_FAIL 0											//位错误事件
#define DAILY_REPAIR 1										//日常修复事件
#define BIT_REPAIRED 2										//位修复事件
#define AUTO_CLEAR 3										//自动清洗事件

//method
#define EDAC_39_32 0										//EDAC(39, 32)
#define BCH_XOR 1											//BCH + XOR两级编码

//stripe info
#define DATA_LEN 32											//数据位长度
#define PARITY_LEN 7										//校验位长度
#define STRIPE_LEN 39										//条纹长度
#define TOLERANCE 2											//容错能力
#define XOR_LEN 6											//两级冗余：第二级冗余条纹长度

#define THRESHOLD 36500										//阈值，100年不出问题，就认为超可靠
#define MEM_SIZE (2*8*1024*1024)							//内存大小
#define STRIPE_NUMBER (2*8*1024*1024/39)					//多少条纹
#define MTTF 20000000										//平均故障时间
#define MTTR 2												//平均修复时间
#define REPS 10												//试验次数
#define CLEAR_CYCLE 10										//自动清洗周期
#define SEU_MBU 2500										//多少次SEU会产生一次MBU, 2500表示2500次SEU会发生一次MBU, 0表示不会出现MBU


typedef struct {
    int type;												//事件类型：磁盘故障/修复
    long time;												//事件发生时间
    int bit_no;												//位号
} event;
event event_heap[MEM_SIZE * 2];								//事件 为什么要乘以2？
int events;													//事件数目

int failed_bits[MEM_SIZE];									//故障位队列
int front, end;												//故障位队列的头和尾

int error_count[STRIPE_NUMBER + 1];							//每个条纹错误位数，多一个防止余数越界
int error_count_xor[STRIPE_NUMBER / XOR_LEN + 1];			//两级冗余：大条纹中有多少个小条纹出现了不可修复的错误

const gsl_rng_type *T;										//gsl random number generator type
gsl_rng *r;													//gsl random number generator

double result[REPS];										//保存每次实验的结果

/**
 *	检查内存中是否出现不可修复的错误
 */
int isFailed(int n, int method)
{
	int i;
	memset(error_count, 0, sizeof(int) * (STRIPE_NUMBER + 1));
	memset(error_count_xor, 0, sizeof(int) * (STRIPE_NUMBER / XOR_LEN + 1));
	if(((end + MEM_SIZE - front) % MEM_SIZE) > 1) {										//统计每个条纹内出现错误的位数
		for(i = front; i != (end + MEM_SIZE) % MEM_SIZE; i ++, i %= MEM_SIZE) {
			error_count[failed_bits[i] / STRIPE_LEN] ++;
			//printf("%ul ", failed_bits[i] / STRIPE_LEN);
		}
		//printf("\n\n");
	}
	
	if(method == EDAC_39_32) {
		for(i = 0; i < STRIPE_NUMBER + 1; i++) {										//一个条纹如果出现一个以上错误，则无法修复
			if(error_count[i] > 1) {
				return 1;
			}
		}
	}

	if(method == BCH_XOR) {																//两级冗余：如果组内有超过一个条纹出现了两个以上的错误，则无法修复
		for(i = 0; i < STRIPE_NUMBER + 1; i++) {
			if(error_count[i] > TOLERANCE) {
				error_count_xor[i / XOR_LEN] ++;
				if(error_count_xor[i / XOR_LEN] > 1) {
					return 1;
				}
			}
		}
	}

	return 0;
}

/**
 *	事件插入最小堆，即优先队列
 */
void heapInsert(long time, int type, int bit_no)
{
    
    int i, p;
    int j;
	
	/**
	for (j=1; j <= events; j++) {													//检查有无重复,待会儿记得删。。。。。。。。
		if (event_heap[j].type == type && event_heap[j].bit_no == bit_no)
	   	{
	   		return;
	   	}
	}
	**/

    events ++;
    i = events;
    p = i / 2;
    while (p > 0 && event_heap[p].time > time) {									//插入到最后，再调整到合适位置
        event_heap[i] = event_heap[p];
        i = p;
        p = i / 2;
    }
    event_heap[i].time = time;
    event_heap[i].type = type;
    event_heap[i].bit_no = bit_no;
   
}

/**
 *	在最小堆查找元素
 */
int heapSearch(int type, int bit_no)
{
  	int i;
  	for (i = 1; i <= events; i++)
  	{
    	if (event_heap[i].type == type && event_heap[i].bit_no == bit_no)
     	{
        	return i;
     	}
  	}
  	return -1;
}

/**
 *	维护最小堆
 *	如果当前元素比孩子节点大，递归更新最小堆
 */
static void __MinHeapIfy(int i)
{
    int min=i;
    int left= 2 * i;
    int right = left + 1;
    
    if(left <= events && event_heap[i].time > event_heap[left].time)
    {
        min = left;
    }
    else
    {
        min = i;
    }
    if(right <= events && event_heap[min].time > event_heap[right].time)
    {
        min=right;
    }
    if(i != min)
    {
        event tmp = event_heap[i];
        event_heap[i] = event_heap[min];
        event_heap[min] = tmp;
        __MinHeapIfy(min);
    }
}

/**
 *	x不在正确位置，x比父节点小
 *	向上递归找它应该在的位置
 */
static void __HeapDecreaseKey(int x)
{
    event tmp;
    while(x > 1 && event_heap[x>>1].time > event_heap[x].time)
    {
        tmp = event_heap[x];
        event_heap[x] = event_heap[x>>1];
        event_heap[x>>1] = tmp;
        x >>= 1;
    }
}

/**
 *	遍历，检查堆是否是最小堆
 */
int __CheckHeap()
{
    int i, left, right;
    int end = events>>1;
    for(i = 1; i <= end; ++ i)
    {
        if(event_heap[i].time < 0)
			return 0;
        left = 2 * i;
        right = left + 1;
        if(left <= events && event_heap[i].time > event_heap[left].time)
            return 0;
        if(right <= events && event_heap[i].time > event_heap[right].time)
            return 0;        
    }
    return 1;
}

/**
 *	删除第i个元素，并将第i个元素赋给event_heap[0]
 */
void heapDelete(int i)
{
    
    event_heap[0] = event_heap[i];
    event tmp = event_heap[events];
    
	//要删除的元素和最后一个元素可能不在同一个树枝上，因此三种情况都有可能哦
    if(event_heap[i].time == tmp.time)										//如果要删除的元素和最后一个相等
    {
        event_heap[i] = tmp;
        events --;
    }
    else if(event_heap[i].time < tmp.time)									//如果要删除的元素比最后一个小
    {
        event_heap[i] = tmp;
        events --;
        __MinHeapIfy(i);
    }
    else 																	//如果要删除的元素比最后一个大
    {
        event_heap[i] = tmp;
       	events --;
       	__HeapDecreaseKey(i);
    }
    
}

/**
 * 	初始化工作
 */
void initialize(int bit_number, double mttf)
{
    int i;
    long life;
    struct timeval tp;
	
	srand((unsigned)time(NULL));
	
    gsl_rng_env_setup();												//初始化gsl
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    gettimeofday(&tp, NULL);
    gsl_rng_set(r, tp.tv_usec);											//set seed，实现真正的随机生成

    events = 0;
	int percent = 0;
    for (i = 0; i < bit_number; i ++) {									//遍历，给每个盘生成一个故障事件，并按发生时间插入到最小堆
        life = gsl_ran_exponential(r, mttf);								//生成指数分布随机数
        heapInsert(life, BIT_FAIL, i);
		
		if (SEU_MBU != 0 && rand() % SEU_MBU < 1) {
			//printf("MBU happened at %ul\n", life);
			if ((++ i) < bit_number) {
				heapInsert(life, BIT_FAIL, i);							//将下一个位用相同的时间置为故障位
			}
		}

		if (i > percent * (bit_number / 100)) {
			printf("Data Loading ...  %d\% \n", percent ++);
		}
    }
}

/**
 *	蒙特卡洛模拟仿真，得到一次仿真的mttdl
 */
double sim_mttdl(int method, int bit_number, double mttf, double mttr)
{
    long life, time_mttr;
    int number;
	long last_clear = 0;																			//上一次自动清洗时间

    while (1) {
        heapDelete(1);
		
		if (event_heap[0].time > THRESHOLD) {
			printf("The Memory has runned for %d years successfully!\n", THRESHOLD / 365);
			return (double)event_heap[0].time;
		}

		if (event_heap[0].time > last_clear) {														//生成自动清洗事件，根据事件发生时间确定自动清洗时间。自动清洗周期为CLEAR_CYCLE。
			last_clear = (long)(event_heap[0].time / CLEAR_CYCLE + 1) * CLEAR_CYCLE;
			heapInsert(last_clear, AUTO_CLEAR, 0);
			printf("auto clear at day %lld\n", last_clear);
		}
		
		if (event_heap[0].type == AUTO_CLEAR) {														//自动清洗事件
			if (isFailed(bit_number, method)) {														//自动清洗前，先检查这段时间是否出现了不可修复的故障
				printf("error found when auto clearing on day: %ld \n", event_heap[0].time);
				return (double)event_heap[0].time;
			}
			int i = 0;
			for (i = front; i != (end + MEM_SIZE) % MEM_SIZE; i ++, i %= MEM_SIZE) {				//清空错误队列，重新生成错误事件
		  		life = gsl_ran_exponential(r, mttf) + last_clear;
				heapInsert(life, BIT_FAIL, failed_bits[i]);

				if (SEU_MBU != 0 && rand() % SEU_MBU < 1) {											//如果产生MBU
					int j = 0;
					int is_next_failed = 0;
					for (j = i + 1; j != end; j ++, j %= MEM_SIZE) {									//检查故障位的第二位（failed_bits[i] + 1）是否在故障队列里
						if (failed_bits[j] == failed_bits[i] + 1) {										//如果故障位的第二位在故障队列里，从故障队列取出并重新生成故障事件（时间同第一位）
							is_next_failed = 1;
							heapInsert(life, BIT_FAIL, failed_bits[j]);										//将故障位的第二位插入最小堆	
							
							failed_bits[j] = failed_bits[(end - 1 + MEM_SIZE) % MEM_SIZE];					//将故障位的第二位从故障队列删除
							end --;
							end = (end + MEM_SIZE) % MEM_SIZE;
						}
					}

					if (is_next_failed == 0) {																//如果故障位的第二位在最小堆里，从最小堆删除并重新生成故障事件（时间同第一位）
						int index_next = 0;
						index_next = heapSearch(BIT_FAIL, failed_bits[i] + 1);								//查找故障位的第二位
						heapDelete(index_next);																//删除故障位的第二位
						heapInsert(life, BIT_FAIL, failed_bits[i] + 1);										//重新生成故障位的第二位
					}
				}
		   	}
			front = 0;
			end = 0;
		}
     	else if (event_heap[0].type == BIT_REPAIRED) {												//位修复事件
		    int flag=0;																				//该修复事件的磁盘的状态：1坏位，2警告了但没坏
			int i;
			if ((end + MEM_SIZE - front) % MEM_SIZE > 0) {
			   	for (i = front; i != end; i ++, i %= MEM_SIZE) {									//检查坏位队列，是否有该位，有flag=1
			   		if (event_heap[0].bit_no == failed_bits[i]) {
			        	flag = 1;
				    	break;
					}
				}
				if (i != end ) {																	//如果坏位队列有该位，就把该位前面的移到后面去，前面时间小?
				 	for (; front != i; front ++, front %= MEM_SIZE)	{
				    	failed_bits[end] = failed_bits[front];
						end ++;
						end %= MEM_SIZE;
				  	}	
					front++;
					front %= MEM_SIZE;
			    }
			}		
			
            if (flag == 1) {																		//如果是坏位，修复并生成下一次坏掉的事件，插入最小堆			   
                life = gsl_ran_exponential(r, mttf) + event_heap[0].time;
                heapInsert(life, BIT_FAIL, event_heap[0].bit_no);	
            }   
            else {																					//有bug，退出
			    printf("error in fail repaired %d, failed_time %ld\n", event_heap[0].bit_no,event_heap[0].time);
				printf("failed number %d \n", (end + MEM_SIZE - front) % MEM_SIZE);
				printf("front = %d, end = %d \n", front, end);
				for (; front != end; front++, front %= MEM_SIZE) 
			      	printf("failed %d \n",failed_bits[front]);
			  	exit(1);
			}
        }
		else if (event_heap[0].type == BIT_FAIL) {													//位错误事件
            failed_bits[end] = event_heap[0].bit_no;												//加入坏位队列
            end ++;
            end %= MEM_SIZE;

			//time_mttr = gsl_ran_exponential(r, mttr);                                               //生成位修复事件
			//heapInsert(event_heap[0].time + time_mttr, BIT_REPAIRED, event_heap[0].bit_no);
		}
        else {
            printf("BUG\n");
            exit(-1);
        }
    }
}

void test(int Method, int Memsize, int Reps, int MBU, int SEU)
{
	int bit_number, reps;
    int mttf, mttr, mttdl;
    int i;
	int method;
	
    mttf = MTTF;
	mttr = MTTR;
	bit_number = Memsize;
	reps = Reps;
	//method = EDAC_39_32;	
	if(Method == 1){
		method = BCH_XOR;
	}else if(Method == 0){
		method= EDAC_39_32;	
	}else {
		printf("Please choose the method of definition from 'BCH_XOR' or 'EDAC' \n");
		return;
	}																		//默认采用EDAC_39_32
	

    mttdl = 0.0;

	printf("bit_number = %d \n", bit_number);
	printf("mttf = %d \n", mttf);
	printf("mttr = %d \n", mttr);
	printf("reps = %d \n", reps);
	
	//执行循环，进行实验
    for (i = 0; i < reps; i++) {	
        initialize(bit_number, mttf);
        front = 0;
        end = 0;
        result[i]=(double)sim_mttdl(method, bit_number, mttf, mttr);
    }
    
    for (i = 0; i < reps; i++) {
		printf("mttdl %d = %lf\n", i + 1, result[i]);
	}
	mttdl = (int)gsl_stats_mean(result, 1, reps);
    printf("\naverage = %d\n", mttdl);
    printf("\nvariance = %lf\n", gsl_stats_variance_with_fixed_mean(result, 1, reps, mttdl));
    gsl_rng_free(r);

}


/**
 * [main description]
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 * mttf         错误平均时间
 * mttr         修复平均时间
 * mttdl        数据丢失平均时间
 * reps         循环次数
 */
int main(int argc, char *argv[])
{
    int bit_number, reps;
    int mttf, mttr, mttdl;
    int i;
	int method;
	
    mttf = MTTF;
	mttr = MTTR;
	bit_number = MEM_SIZE;
	reps = REPS;
	//method = EDAC_39_32;																			//默认采用EDAC_39_32
	method = BCH_XOR;

    if (argc == 2) {
        printf("Usage: monte reps\n");
		reps = atoi(argv[1]);
    }

    mttdl = 0.0;

	printf("bit_number = %d \n", bit_number);
	printf("mttf = %d \n", mttf);
	printf("mttr = %d \n", mttr);
	printf("reps = %d \n", reps);
	
	//执行循环，进行实验
    for (i = 0; i < reps; i++) {	
        initialize(bit_number, mttf);
        front = 0;
        end = 0;
        result[i]=(double)sim_mttdl(method, bit_number, mttf, mttr);
    }
    
    for (i = 0; i < reps; i++) {
		printf("mttdl %d = %lf\n", i + 1, result[i]);
	}
	mttdl = (int)gsl_stats_mean(result, 1, reps);
    printf("\naverage = %d\n", mttdl);
    printf("\nvariance = %lf\n", gsl_stats_variance_with_fixed_mean(result, 1, reps, mttdl));
    gsl_rng_free(r);
    return 0;
}

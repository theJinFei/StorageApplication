#include "HammingCode.h"
#include <cstdlib>
//#include <sys/time.h>
#include <cstring>
//#include <unistd.h>
#include <random>

bitset<dataLen> enInput("00001100000001000100010000000000");
bitset<codeLen> enOutput;
bitset<codeLen> deInput;
bitset<codeLen> deOutput;
bitset<parityLen> parityXOR;

//const unsigned long number_Group = 67000000; //设置的数据量 67108864组大概是1GB的数据量
const unsigned long number_Group = 1; //设置的数据量 67108864组大概是1GB的数据量
vector<bitset<dataLen> > testSecInput(number_Group * stripesize, bitset<dataLen>(0)); //输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecOutput(number_Group * stripesize, bitset<codeLen>(0)); //输出 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeInput(number_Group * stripesize, bitset<codeLen>(0)); //进行编码 输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeOutput(number_Group * stripesize, bitset<codeLen>(0)); //进行编码 输出 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testXOR(number_Group, bitset<codeLen>(0));
vector<bitset<codeLen> > testXORDeInput(number_Group, bitset<codeLen>(0));
vector<bitset<codeLen> > testXORDeOutput(number_Group, bitset<codeLen>(0));

unordered_map<bitset<parityLen>, bitset<codeLen>> errTable;
unsigned long flag[number_Group * stripesize] = { 0 };

time_t timeBegin_Read, timeEnd_Read;
time_t timeBegin_Write, timeEnd_Write;

int array_RulesGenenate[dataLen] =
	{ 0b1001111,
	0b1001010,
	0b1010010,
	0b1010100,
	0b1010111,
	0b1011000,
	0b1011011,
	0b1011101,
	0b0100011,
	0b0100101,
	0b0100110,
	0b0101001,
	0b0101010,
	0b0101100,
	0b0110001,
	0b0110100,
	0b0001110,
	0b0001011,
	0b0010011,
	0b0010101,
	0b0010110,
	0b0011001,
	0b0011010,
	0b0011100,
	0b1100010,
	0b1100100,
	0b1100111,
	0b1101000,
	0b1101011,
	0b1101101,
	0b1110000,
	0b1110101 };

int array_RulesParity[codeLen] = {
	0b1000000,
	0b0100000,
	0b0010000,
	0b0001000,
	0b0000100,
	0b0000010,
	0b0000001,
	0b1111001,
	0b0101001,
	0b0100101,
	0b0010101,
	0b1110101,
	0b0001101,
	0b1101101,
	0b1011101,
	0b1100010,
	0b1010010,
	0b0110010,
	0b1001010,
	0b0101010,
	0b0011010,
	0b1000110,
	0b0010110,
	0b0111000,
	0b1101000,
	0b1100100,
	0b1010100,
	0b0110100,
	0b1001100,
	0b0101100,
	0b0011100,
	0b0100011,
	0b0010011,
	0b1110011,
	0b0001011,
	0b1101011,
	0b1011011,
	0b0000111,
	0b1010111 };
/**
Hamming编码

@param input 输入32位信息位
@param output 39位编码后的Hamming码
*/
void HammingCode::HammingEncode(bitset<dataLen> & input, bitset<codeLen> & output) {
	//printf("HammingEncode start...\n");

	//    //进行赋值操作
	//    for(int i = 0; i < dataLen; i++) {
	//        output[i + 12] = input[i];
	//    }
	//    output[0] = input[0] ^ input[3] ^ input[4] ^ input[5] ^ input[6] ^ input[9] ^ input[10] ^ input[12] ^ input[16] ^ input[17] ^ input[21] ^ input[22] ^ input[24] ^ input[25] ^ input[26] ^ input[27] ^ input[28] ^ input[29];                                //p0
	//    output[1] = input[1] ^ input[4] ^ input[5] ^ input[6] ^ input[7] ^ input[10] ^ input[11] ^ input[13] ^ input[17] ^ input[18] ^ input[22] ^ input[23] ^ input[25] ^ input[26] ^ input[27] ^ input[28] ^ input[29] ^ input[30];                               //p1
	//    output[2] = input[0] ^ input[2] ^ input[3] ^ input[4] ^ input[7] ^ input[8] ^ input[9] ^ input[10] ^ input[11] ^ input[14] ^ input[16] ^ input[17] ^ input[18] ^ input[19] ^ input[21] ^ input[22] ^ input[23] ^ input[25] ^ input[30] ^ input[31];         //p2
	//    output[3] = input[1] ^ input[3] ^ input[4] ^ input[5] ^ input[8] ^ input[9] ^ input[10] ^ input[11] ^ input[12] ^ input[15] ^ input[17] ^ input[18] ^ input[19] ^ input[20] ^ input[22] ^ input[23] ^ input[24] ^ input[26] ^ input[31];                    //p3
	//    output[4] = input[0] ^ input[2] ^ input[3] ^ input[11] ^ input[13] ^ input[17] ^ input[18] ^ input[19] ^ input[20] ^ input[22] ^ input[23] ^ input[26] ^ input[28] ^ input[29];                                                                             //p4
	//    output[5] = input[1] ^ input[3] ^ input[4] ^ input[12] ^ input[14] ^ input[18] ^ input[19] ^ input[20] ^ input[21] ^ input[23] ^ input[24] ^ input[27] ^ input[29] ^ input[30];                                                                             //p5
	//    output[6] = input[2] ^ input[4] ^ input[5] ^ input[13] ^ input[15] ^ input[19] ^ input[20] ^ input[21] ^ input[22] ^ input[24] ^ input[25] ^ input[28] ^ input[30] ^ input[31];                                                                             //p6
	//    output[7] = input[0] ^ input[4] ^ input[9] ^ input[10] ^ input[12] ^ input[14] ^ input[17] ^ input[20] ^ input[23] ^ input[24] ^ input[27] ^ input[28] ^ input[31];                                                                                         //p7
	//    output[8] = input[0] ^ input[1] ^ input[3] ^ input[4] ^ input[6] ^ input[9] ^ input[11] ^ input[12] ^ input[13] ^ input[15] ^ input[16] ^ input[17] ^ input[18] ^ input[22] ^ input[26] ^ input[27];                                                        //p8
	//    output[9] = input[0] ^ input[1] ^ input[2] ^ input[3] ^ input[6] ^ input[7] ^ input[9] ^ input[13] ^ input[14] ^ input[18] ^ input[19] ^ input[21] ^ input[22] ^ input[23] ^ input[24] ^ input[25] ^ input[26] ^ input[29];                                 //p9
	//    output[10] = input[1] ^ input[2] ^ input[3] ^ input[4] ^ input[7] ^ input[8] ^ input[10] ^ input[14] ^ input[15] ^ input[19] ^ input[20] ^ input[22] ^ input[23] ^ input[24] ^ input[25] ^ input[26] ^ input[27] ^ input[30];                               //p10
	//    output[11] = input[2] ^ input[3] ^ input[4] ^ input[5] ^ input[8] ^ input[9] ^ input[11] ^ input[15] ^ input[16] ^ input[20] ^ input[21] ^ input[23] ^ input[24] ^ input[25] ^ input[26] ^ input[27] ^ input[28] ^ input[31];                               //p11

	int res = 0b0;
	for (int i = 0; i < dataLen; i++) {
		if (input[i]) {
			res ^= array_RulesGenenate[i];
		}
	}
	output = res;
	//进行赋值操作
	for (int i = 0; i < dataLen; i++) {
		output[i + parityLen] = input[i];
	}


	//printf("HammingEncode end...\n");
}


void HammingCode::HammingDecode(bitset<codeLen> & input, bitset<codeLen> & output) {
	getParityXOR(input, parityXOR);
	//如果没有错误
	if (!parityXOR.any()) {
		output = input;
		return;
	}
	//如果errTable中有该错误模式，则修复
	auto existTable = errTable.find(parityXOR);
	if (existTable != errTable.end()) {
		output = input ^ errTable[parityXOR];
	}
	else {
		output = input;
		//cout << "\tToo many errors, and it can not be repaired" << endl;
	}
}


/**
求错误伴随式

@param input 接收到的数据
@param output 错误伴随式 生成的十二位校验子
*/
void HammingCode::getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output) {
	//校验规则
	// 38 37 36 35  34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5 4   3  2  1  0
	//{ 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 }, //a6
	//{ 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0 }, //a5	
	//{ 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 }, //a4	
	//{ 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0 }, //a3	
	//{ 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, //a2	
	//{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, //a1	
	//{ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 }, //a0	

	int res = 0b0;
	for (int i = 0; i < codeLen; i++) {
		if (input[i]) {
			res ^= array_RulesParity[i];
		}
	}
	output = res;
}


/**
制表，保存所有的1位或2位错误导致的新旧校验位异或值
*/
void HammingCode::setTable() {
	//生成文件流
	string fileName = "errTableHamming_44_32.txt";
	ofstream fout(fileName, ios::binary);
	fout << "Loc\t" << "XOR\t" << "\n";

	//编码
	//cout << enInput << endl;
	HammingEncode(enInput, enOutput);
	//cout << enOutput << endl;

	//出现1位错误，错误位置位errLoc
	for (int errLoc = 0; errLoc < codeLen; errLoc++) {
		//memcpy(deInput, enOutput, codeLen * sizeof(int));
		deInput = enOutput;                                                                                                                                                                                                                                                                      
		deInput.flip(errLoc);             //翻转第errLoc位

		getParityXOR(deInput, parityXOR);

		bitset<codeLen> tmp;
		tmp.flip(errLoc);
		fout << tmp << "\t" << parityXOR << "\t" << "\n";
		//fout << parityXOR.to_ullong() << "\n";
	}
}


/**
读表，并倒排保存到errTable中，下标为伴随式，值为错误位置
*/
void HammingCode::getTable() {
	//生成文件流
	string fileName = "errTableHamming_44_32.txt";
	ifstream fin(fileName, ios::binary);
	string tmpStr;
	bitset<codeLen> tmp1;
	bitset<parityLen> tmp2;
	fin >> tmpStr >> tmpStr;            //去表头
	while (!fin.eof()) {
		fin >> tmp1 >> tmp2;
		errTable[tmp2] = tmp1;
	}
}



/**
Hamming_testOneorTwo,封装一下
*/
void HammingCode::ErrorCorrection() {
	for (int i = 0; i < testSecDeOutput.size(); i++) {
		if (!flag[i]) {
			HammingDecode(testSecDeInput[i], testSecDeOutput[i]);
		}
		if (testSecDeOutput[i] == testSecOutput[i]) {
			testSecDeInput[i] = testSecDeOutput[i]; //第一级编码纠正正确，记得有一步是写回操作
			cout << "\tUsing the inner HammingDncode_" << i << " success and the sequence after repair is: \t" << testSecDeOutput[i] << endl;
		}
		else {
			//如果只有1个序列其中的1位以上发生了错误，则抛给异或进行解码
			//cout << "\tThe sequence_"<< i << " " << testSecDeInput[i] << " have more than 2 errors, enter the second level of redundancy check:\t" << endl;
			testSecDeOutput[i].reset(); //把原来的值清0，进入第二级校验
			//cout << "\tThe first number of group is: i / stripesize * stripesize = " << i / stripesize * stripesize << endl;
			int Group = i / stripesize; //i除以stripesize，看具体在哪一组，第二级的校验要用到
			HammingDecode(testXORDeInput[Group], testXORDeOutput[Group]); //对对应组的XOR进行解码
			if (testXORDeOutput[Group] == testXOR[Group]) { //证明本组的XOR无错误
			//cout <<"\tThe XOR:\t"<< testXORDeOutput[Group] << " has no errors" << endl;
				//做异或校验
				for (int k = Group * stripesize; k < Group * stripesize + stripesize; k++) {
					if (k != i && !flag[k]) {
						HammingDecode(testSecDeInput[k], testSecDeOutput[k]); //对本组的其他条纹进行解码
						flag[k] = 1; //标记第k个条纹是否进行了解码
					}
					if (k != i) {
						//cout << "\tthe sequence k " << k << " in outer check" << endl;
						//for (int j = 0; j < blocksize; j++) {
						//	testSecDeOutput[i][j] = testSecDeOutput[i][j] ^ testSecDeOutput[k][j];
						//}
						testSecDeOutput[i] ^= testSecDeOutput[k];
					}
				}
				//for (int j = 0; j < blocksize; j++) {
				//	testSecDeOutput[i][j] = testSecDeOutput[i][j] ^ testXORDeOutput[Group][j];
				//}
				testSecDeOutput[i] ^= testXORDeOutput[Group];
				if (testSecDeOutput[i] == testSecOutput[i]) {
					testSecDeInput[i] = testSecDeOutput[i]; //第二级编码纠正正确，记得有一步是写回操作
					cout << "\tUsing the outer XORDecode_"<< i <<" success and the sequence after repair is:\t" << testSecDeOutput[i] <<endl;
				}
				else {
					//cout << "\tRepair failed, testSecDeOutput is : " << testSecDeOutput[i] << endl;
					cout << "\tThere are more than two errors occurred in more than two sequences, repair failed.\t" << endl;
				}
			}
			else {
				//cout << "\tThe XOR have more than 2 errors, repair failed:\t " << testXORDeOutput[i / 4] << endl;
			}
		}
	}
}


/**
* 测试1位错误 所用时间
*/
void HammingCode::testOneError() {
	//Time_consuming_Read = 0;
	//一组编码中  每个条纹生成1位错误
	cout << "$4:\ttest1: gengerate one error:\t" << endl;
	for (int n = 0; n < stripesize; n++) {
		testSecDeInput = testSecOutput;
		for (int errLoc1 = 0; errLoc1 < codeLen; errLoc1++) {
			testSecDeInput[n].flip(errLoc1);

			cout << endl;
			cout << "\tThe sequence_" << n << " after insert error " << "at " << errLoc1 << " is:\t";
			cout << testSecDeInput[n] << endl;

			//实现读的计时
			//gettimeofday(&start_time, NULL);
			ErrorCorrection();
			//gettimeofday(&end_time, NULL);
			//Time_consuming_Read += 1000000 * (end_time.tv_sec - start_time.tv_sec) + end_time.tv_usec - start_time.tv_usec;
		}
	}
	//cout << "\tFunction of gengerate one error and decode them execution time is " << (double)Time_consuming_Read / 1000000 << " s" << endl;
}

/**
* 测试2位错误 所用时间
*/
void HammingCode::testTwoErrors() {
	//Time_consuming_Read = 0;
	//每个条纹里生成2位错误
	cout << "$5:\ttest2: gengerate two errors:\t" << endl;
	for (int n = 0; n < stripesize; n++) {
		for (int errLoc1 = 0; errLoc1 < codeLen - 1; errLoc1++) {
			for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen; errLoc2++) {
				testSecDeInput[n].flip(errLoc1);
				testSecDeInput[n].flip(errLoc2);

				cout << endl;
				cout << "\tThe sequence_" << n << " after insert errors " << "at " << errLoc1 << "," << errLoc2 << " is:\t";
				cout << testSecDeInput[n] << endl;

				//实现读的计时
				//gettimeofday(&start_time, NULL);
				ErrorCorrection();
				//gettimeofday(&end_time, NULL);
				//Time_consuming_Read += 1000000 * (end_time.tv_sec - start_time.tv_sec) + end_time.tv_usec - start_time.tv_usec;
			}
		}
	}
	//cout << "\tFunction of gengerate two errors and decode them execution time is " << (double)Time_consuming_Read / 1000000 << " s" << endl;
}



void HammingCode::testFunctional() {
	//testWrite(); //生成编码
	//testOneError();  //测试1位错误 所用时 函数
	//testTwoErrors();  //测试2位错误 所用时 函数
}


/**
* 测试读 所用时间
*/
void HammingCode::testRead() {
	//每个条纹里随机生成[0,3]个错误
	for (int i = 0; i < testSecDeInput.size(); i++) {
		int m = 0, n = 2;
		int temp = rand() % (n - m + 1) + m; //生成[m, n]之间的随机数 temp为testSecDeInput错误的个数
		for (int j = 0; j < temp; j++) {
			int errLoc = rand() % (codeLen - 1 - 0 + 1) + 0; //随机生成[0,39]的错误位置
			testSecDeInput[i].flip(errLoc); //产生错误的时候，产生temp个随机错误，errLoc为错误位置
		}
		cout << "\tThe sequence_" << i << " after insert "<< temp <<" error(s) " << " is:\t"  << testSecDeInput[i] << endl;
	}


	timeBegin_Read = time(NULL);
	ErrorCorrection();
	timeEnd_Read = time(NULL);

	cout << "\tRead operation time is " << timeEnd_Read - timeBegin_Read << " s" << endl;

}

/**
*   测试读修改写
*/
void HammingCode::testUpdateWrite() {

	/************************************************************************/
	/* 测试用读修改写  
	/* 当校验数据少于条纹组的一半的时候
	/* 使用读修改写，例如只将d0更行为d0',
	/* 只需要读出d0和原有的校验p进行异或即可                                                    
	/************************************************************************/
	for (int j = 0; j < stripesize; j++) { //生成一组随机序列
		testSecInput[j] = ((long)rand()) << 16 + (long)rand();
		//cout << "\tHamming_"<< j << " encoding before:\t" << testSecInput[j] << endl;
		//实现写的计时 其中写包括 随机生成39位信息位 + 计算校验位 + 生成的异或
		HammingEncode(testSecInput[j], testSecOutput[j]);
		cout << "\tHamming_" << j << " encoding after:\t" << testSecOutput[j] << endl;
		int Group = j / stripesize;
		testXOR[Group] ^= testSecOutput[j];
	}
	timeBegin_Write = time(NULL);
	//验证完之后 记得重置 防止后一组的实验对前一组产生影响
	int tmp = 0;
	//int j = rand() % testSecInput.size(); //产生随机条纹 产生[0, testSecInput.size() - 1]下标的条纹
	int j = 0;
	int Group = j / stripesize; //判断在哪一组
	int numStrip_NeedUpdate = rand() % (stripesize - 1 + 1) + 1 + Group * stripesize; //由上面生成的组，随机选择一个条纹进行更新
	testXOR[Group] ^= testSecOutput[numStrip_NeedUpdate]; //实现XOR与d0的异或 先要访问内存两次
	testSecInput[numStrip_NeedUpdate] = ((long)rand()) << 16 + (long)rand(); //随机生成新的d0
	HammingEncode(testSecInput[numStrip_NeedUpdate], testSecOutput[numStrip_NeedUpdate]); //编码
	cout << "\tUpdate Hamming_\t" << numStrip_NeedUpdate << " after:\t" << testSecOutput[numStrip_NeedUpdate] << endl;
	testXOR[Group] ^= testSecOutput[numStrip_NeedUpdate]; //再与上式异或后的结果进行异或
	cout << "\ttestXOR[" << Group << "] is: \t\t\t"<< testXOR[Group] << endl;

	testSecDeInput = testSecOutput; //最后进行写入
	testXORDeInput = testXOR;
	timeEnd_Write = time(NULL);
	cout << "\tUpdate operation time is " << timeEnd_Write - timeBegin_Write << " s" << endl;
}



/**
*   测试写
*/
void HammingCode::testWrite() {
	timeBegin_Write = time(NULL);
	//验证完之后 记得重置 防止后一组的实验对前一组产生影响
	int tmp = 0;
	for (int j = 0; j < testSecInput.size(); j++) { //生成一组随机序列
		testSecInput[j] = ((long)rand()) << 16 + (long)rand();
		//cout << "\tHamming_"<< j << " encoding before:\t" << testSecInput[j] << endl;
		//实现写的计时 其中写包括 随机生成39位信息位 + 计算校验位 + 生成的异或
		HammingEncode(testSecInput[j], testSecOutput[j]);
		cout << "\tHamming_" << j << " encoding after:\t" << testSecOutput[j] << endl;
		int Group = j / stripesize;
		testXOR[Group] ^= testSecOutput[j];
	}
	for(int i = 0; i < testXOR.size(); i++){
	    cout << "\ttestXOR_" << i << " is: \t\t\t" << testXOR[i] << endl;
	}

	testSecDeInput = testSecOutput;
	testXORDeInput = testXOR;
	timeEnd_Write = time(NULL);
	cout << "\tWrite operation time is " << timeEnd_Write - timeBegin_Write << " s" << endl;
}



/**
Hamming编码测试程序
*/
void HammingCode::testHamming() {
	//cout << "$1:\tsetTable(make table and saved errTableHamming_44_32.txt)" << endl;
	setTable();
	//cout << "$2:\tgetTable(read from errTableHamming_44_32.txt and saved errTable arrays)" << endl;
	getTable();

	//testFunctional(); //功能性测试

	srand((unsigned int)time(NULL)); //srand()一定要放在循环外面或者是循环调用的外面，否则的话得到的是相同的随机数

	testWrite(); //测试写用时 目前包括 生成39位随机数的值 + Hamming32位信息码 变为 39位信息码的过程
	testRead();  //测试读 所用时 包括校验的过程函数

	//cout << "\tTotal operation Time is " << timeEnd_Write - timeBegin_Write + timeEnd_Read - timeBegin_Read << " s" << endl;
}




/**
测试出现3个位错误时，会不会有冲突。枚举3个位错误的情况，然后查errTable表，如果能查到，说明有冲突。
如果有，这种编码方案就不适合做字内编码

md结果证明真的有冲突😢
find conflict at:     000000010011    for err:     00000000000000000000000000000000000000010011
find conflict at:     000100000011    for err:     00000000000000000000000000000000000100000011
find conflict at:     001000000011    for err:     00000000000000000000000000000000001000000011
find conflict at:     111001010111    for err:     00000000000000000000000000000100000000000011
find conflict at:     110111101100    for err:     00000000000000000000000000010000000000000011
......
*/
void HammingCode::errTest() {

	cout << "$1:\tsetTable(制作错误位置表，保存到errTable.txt)" << endl;
	setTable();
	cout << "$2:\tgetTable(从errTableHamming_44_32.txt读去错误模式，并倒排保存到errTable数组中，下标为伴随式，值为错误位置)" << endl;
	getTable();

	cout << "$3:\terrTest(遍历3个错误的情况，得到新旧校验异或，并查询errTable，如果存在，则有冲突)" << endl;
	for (int errLoc1 = 0; errLoc1 < codeLen - 2; errLoc1++) {
		for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen - 1; errLoc2++) {
			for (int errLoc3 = errLoc2 + 1; errLoc3 < codeLen; errLoc3++) {
				deInput.reset();                //deInput置零
				deInput.flip(errLoc1);
				deInput.flip(errLoc2);
				deInput.flip(errLoc3);
				getParityXOR(deInput, parityXOR);
				//输出冲突项
				auto findConflict = errTable.find(parityXOR);
				if (findConflict != errTable.end()) {
					cout << "\tfind conflict at: \t" << findConflict->first << "\tfor err: \t" << deInput << endl;
				}
			}
		}
	}
}

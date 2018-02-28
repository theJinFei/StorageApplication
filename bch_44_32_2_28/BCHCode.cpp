#include "BCHCode.h"
#include <cstdlib>
#include <sys/time.h>
#include <cstring>
#include <unistd.h>
#include <random>

bitset<dataLen> enInput("00001100000000000000000000000000");
bitset<codeLen> enOutput;
bitset<codeLen> deInput;
bitset<codeLen> deOutput;
bitset<parityLen> parityXOR;

const unsigned long number_Group = 67000000; //设置的数据量 67108864组大概是1GB的数据量
//const unsigned long number_Group = 4; //设置的数据量 67108864组大概是1GB的数据量
vector<bitset<dataLen> > testSecInput(number_Group * stripesize, bitset<dataLen> (0)); //输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecOutput(number_Group * stripesize, bitset<codeLen> (0)); //输出 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeInput(number_Group * stripesize, bitset<codeLen> (0)); //进行编码 输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeOutput(number_Group * stripesize, bitset<codeLen> (0)); //进行编码 输出 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testXOR(number_Group, bitset<codeLen> (0));
vector<bitset<codeLen> > testXORDeInput(number_Group, bitset<codeLen> (0));
vector<bitset<codeLen> > testXORDeOutput(number_Group, bitset<codeLen> (0));

unordered_map<bitset<parityLen>, bitset<codeLen>> errTable;
unsigned long flag[number_Group * stripesize] = {0};

struct  timeval  start_time;
struct  timeval  end_time;
unsigned long Time_consuming_Read = 0, Time_consuming_Write = 0;

/**
 BCH编码

 @param input 输入32位信息位
 @param output 输出44位编码后的BCH码
 */
void BCHCode::BCHEncode(bitset<dataLen> & input, bitset<codeLen> & output) {
    //printf("BCHEncode start...\n");

    //进行赋值操作
    for(int i = 0; i < dataLen; i++) {
        output[i + 12] = input[i];
    }
    output[0] = input[0] ^ input[3] ^ input[4] ^ input[5] ^ input[6] ^ input[9] ^ input[10] ^ input[12] ^ input[16] ^ input[17] ^ input[21] ^ input[22] ^ input[24] ^ input[25] ^ input[26] ^ input[27] ^ input[28] ^ input[29];                                //p0
    output[1] = input[1] ^ input[4] ^ input[5] ^ input[6] ^ input[7] ^ input[10] ^ input[11] ^ input[13] ^ input[17] ^ input[18] ^ input[22] ^ input[23] ^ input[25] ^ input[26] ^ input[27] ^ input[28] ^ input[29] ^ input[30];                               //p1
    output[2] = input[0] ^ input[2] ^ input[3] ^ input[4] ^ input[7] ^ input[8] ^ input[9] ^ input[10] ^ input[11] ^ input[14] ^ input[16] ^ input[17] ^ input[18] ^ input[19] ^ input[21] ^ input[22] ^ input[23] ^ input[25] ^ input[30] ^ input[31];         //p2
    output[3] = input[1] ^ input[3] ^ input[4] ^ input[5] ^ input[8] ^ input[9] ^ input[10] ^ input[11] ^ input[12] ^ input[15] ^ input[17] ^ input[18] ^ input[19] ^ input[20] ^ input[22] ^ input[23] ^ input[24] ^ input[26] ^ input[31];                    //p3
    output[4] = input[0] ^ input[2] ^ input[3] ^ input[11] ^ input[13] ^ input[17] ^ input[18] ^ input[19] ^ input[20] ^ input[22] ^ input[23] ^ input[26] ^ input[28] ^ input[29];                                                                             //p4
    output[5] = input[1] ^ input[3] ^ input[4] ^ input[12] ^ input[14] ^ input[18] ^ input[19] ^ input[20] ^ input[21] ^ input[23] ^ input[24] ^ input[27] ^ input[29] ^ input[30];                                                                             //p5
    output[6] = input[2] ^ input[4] ^ input[5] ^ input[13] ^ input[15] ^ input[19] ^ input[20] ^ input[21] ^ input[22] ^ input[24] ^ input[25] ^ input[28] ^ input[30] ^ input[31];                                                                             //p6
    output[7] = input[0] ^ input[4] ^ input[9] ^ input[10] ^ input[12] ^ input[14] ^ input[17] ^ input[20] ^ input[23] ^ input[24] ^ input[27] ^ input[28] ^ input[31];                                                                                         //p7
    output[8] = input[0] ^ input[1] ^ input[3] ^ input[4] ^ input[6] ^ input[9] ^ input[11] ^ input[12] ^ input[13] ^ input[15] ^ input[16] ^ input[17] ^ input[18] ^ input[22] ^ input[26] ^ input[27];                                                        //p8
    output[9] = input[0] ^ input[1] ^ input[2] ^ input[3] ^ input[6] ^ input[7] ^ input[9] ^ input[13] ^ input[14] ^ input[18] ^ input[19] ^ input[21] ^ input[22] ^ input[23] ^ input[24] ^ input[25] ^ input[26] ^ input[29];                                 //p9
    output[10] = input[1] ^ input[2] ^ input[3] ^ input[4] ^ input[7] ^ input[8] ^ input[10] ^ input[14] ^ input[15] ^ input[19] ^ input[20] ^ input[22] ^ input[23] ^ input[24] ^ input[25] ^ input[26] ^ input[27] ^ input[30];                               //p10
    output[11] = input[2] ^ input[3] ^ input[4] ^ input[5] ^ input[8] ^ input[9] ^ input[11] ^ input[15] ^ input[16] ^ input[20] ^ input[21] ^ input[23] ^ input[24] ^ input[25] ^ input[26] ^ input[27] ^ input[28] ^ input[31];                               //p11

    //printf("BCHEncode end...\n");
}


void BCHCode::BCHDecode(bitset<codeLen> & input, bitset<codeLen> & output) {
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
    } else {
        output = input;
        //cout << "\tToo many errors, and it can not be repaired" << endl;
    }
}


/**
 求错误伴随式

 @param input 接收到的数据
 @param output 错误伴随式 生成的十二位校验子
 */
void BCHCode::getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output) {
    output[0] = input[0] ^ input[12] ^ input[15] ^ input[16] ^ input[17] ^ input[18] ^ input[21] ^ input[22] ^ input[24] ^ input[28] ^ input[29] ^ input[33] ^ input[34] ^ input[36] ^ input[37] ^ input[38] ^ input[39] ^ input[40] ^ input[41];                           //a0
    output[1] = input[1] ^ input[13] ^ input[16] ^ input[17] ^ input[18] ^ input[19] ^ input[22] ^ input[23] ^ input[25] ^ input[29] ^ input[30] ^ input[34] ^ input[35] ^ input[37] ^ input[38] ^ input[39] ^ input[40] ^ input[41] ^ input[42];                           //a1
    output[2] = input[2] ^ input[12] ^ input[14] ^ input[15] ^ input[16] ^ input[19] ^ input[20] ^ input[21] ^ input[22] ^ input[23] ^ input[26] ^ input[28] ^ input[29] ^ input[30] ^ input[31] ^ input[33] ^ input[34] ^ input[35] ^ input[37] ^ input[42] ^ input[43];   //a2
    output[3] = input[3] ^ input[13] ^ input[15] ^ input[16] ^ input[17] ^ input[20] ^ input[21] ^ input[22] ^ input[23] ^ input[24] ^ input[27] ^ input[29] ^ input[30] ^ input[31] ^ input[32] ^ input[34] ^ input[35] ^ input[36] ^ input[38] ^ input[43];               //a3
    output[4] = input[4] ^ input[12] ^ input[14] ^ input[15] ^ input[23] ^ input[25] ^ input[29] ^ input[30] ^ input[31] ^ input[32] ^ input[34] ^ input[35] ^ input[38] ^ input[40] ^ input[41];                                                                           //a4
    output[5] = input[5] ^ input[13] ^ input[15] ^ input[16] ^ input[24] ^ input[26] ^ input[30] ^ input[31] ^ input[32] ^ input[33] ^ input[35] ^ input[36] ^ input[39] ^ input[41] ^ input[42];                                                                           //a5
    output[6] = input[6] ^ input[14] ^ input[16] ^ input[17] ^ input[25] ^ input[27] ^ input[31] ^ input[32] ^ input[33] ^ input[34] ^ input[36] ^ input[37] ^ input[40] ^ input[42] ^ input[43];                                                                           //a6
    output[7] = input[7] ^ input[12] ^ input[16] ^ input[21] ^ input[22] ^ input[24] ^ input[26] ^ input[29] ^ input[32] ^ input[35] ^ input[36] ^ input[39] ^ input[40] ^ input[43];                                                                                       //a7
    output[8] = input[8] ^ input[12] ^ input[13] ^ input[15] ^ input[16] ^ input[18] ^ input[21] ^ input[23] ^ input[24] ^ input[25] ^ input[27] ^ input[28] ^ input[29] ^ input[30] ^ input[34] ^ input[38] ^ input[39];                                                   //a8
    output[9] = input[9] ^ input[12] ^ input[13] ^ input[14] ^ input[15] ^ input[18] ^ input[19] ^ input[21] ^ input[25] ^ input[26] ^ input[30] ^ input[31] ^ input[33] ^ input[34] ^ input[35] ^ input[36] ^ input[37] ^ input[38] ^ input[41];                           //a9
    output[10] = input[10] ^ input[13] ^ input[14] ^ input[15] ^ input[16] ^ input[19] ^ input[20] ^ input[22] ^ input[26] ^ input[27] ^ input[31] ^ input[32] ^ input[34] ^ input[35] ^ input[36] ^ input[37] ^ input[38] ^ input[39] ^ input[42];                         //a10
    output[11] = input[11] ^ input[14] ^ input[15] ^ input[16] ^ input[17] ^ input[20] ^ input[21] ^ input[23] ^ input[27] ^ input[28] ^ input[32] ^ input[33] ^ input[35] ^ input[36] ^ input[37] ^ input[38] ^ input[39] ^ input[40] ^ input[43];                         //a11
}


/**
 制表，保存所有的1位或2位错误导致的新旧校验位异或值
 */
void BCHCode::setTable() {
    //生成文件流
    string fileName = "errTableBCH_44_32.txt";
    ofstream fout(fileName, ios::binary);
    fout << "Loc\t" << "XOR\t" << "\n";

    //编码
    //cout << enInput << endl;
    BCHEncode(enInput, enOutput);
    //cout << enOutput << endl;

    //出现1位错误，错误位置位errLoc
    for(int errLoc = 0; errLoc < codeLen; errLoc ++) {
        //memcpy(deInput, enOutput, codeLen * sizeof(int));
        deInput = enOutput;
        deInput.flip(errLoc);             //翻转第errLoc位

        getParityXOR(deInput, parityXOR);

        bitset<codeLen> tmp;
        tmp.flip(errLoc);
        fout << tmp << "\t" << parityXOR << "\t" << "\n";
        //fout << parityXOR.to_ullong() << "\n";
    }

    //出现2位错误，错误位置为errLoc1和errLoc2
    for (int errLoc1 = 0; errLoc1 < codeLen - 1; errLoc1 ++) {
        for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen; errLoc2 ++) {
            //memcpy(deInput, enOutput, codeLen * sizeof(int));
            deInput = enOutput;
            deInput.flip(errLoc1);            //翻转第errLoc1位
            deInput.flip(errLoc2);            //翻转第errLoc2位

            getParityXOR(deInput, parityXOR);

            bitset<codeLen> tmp;
            tmp.flip(errLoc1);
            tmp.flip(errLoc2);
            fout << tmp << "\t" << parityXOR << "\t" << "\n";
            //fout << parityXOR.to_ullong() << "\n";
        }
    }
}


/**
 读表，并倒排保存到errTable中，下标为伴随式，值为错误位置
 */
void BCHCode::getTable() {
    //生成文件流
    string fileName = "errTableBCH_44_32.txt";
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
 BCH_testOneorTwo,封装一下
 */
void BCHCode::ErrorCorrection() {
//    for(int i = 0; i < testSecDeOutput.size(); i++) {
//        BCHDecode(testSecDeInput[i], testSecDeOutput[i]);
//    }
    for(int i = 0; i < testSecDeOutput.size(); i++) {
        if(!flag[i]){
            BCHDecode(testSecDeInput[i], testSecDeOutput[i]);
        }
        if(testSecDeOutput[i] == testSecOutput[i] ) {
            //cout << "\tUsing the inner BCHEncode_" << i <<" success and the sequence after repair is: \t" << testSecDeOutput[i] <<endl;
        } else if(testSecDeOutput[i] != testSecOutput[i]) {
            //如果只有1个序列其中的2位以上发生了错误，则抛给异或进行解码
            //cout << "\tThe sequence_"<< i << " " << testSecDeOutput[i] << " have more than 2 errors, enter the second level of redundancy check:\t" << endl;
            testSecDeOutput[i].reset(); //把原来的值清0，进入第二级校验
            //cout << "\tThe first number of group is: i / 4 * 4 = " << i / 4 * 4 << endl;
            int Group = i / stripesize; //i右移了两位，除以4，看具体在哪一组，第二级的校验要用到
            BCHDecode(testXORDeInput[Group], testXORDeOutput[Group]); //对对应组的XOR进行解码
            if(testXORDeOutput[Group] == testXOR[Group]) { //证明本组的XOR无错误
                //cout <<"\tThe XOR:\t"<< testXORDeOutput[Group] << " has no errors" << endl;
                //做异或校验
                for(int k = Group * stripesize; k < Group * stripesize + stripesize; k++) {
                    if(k != i) {
                        BCHDecode(testSecDeInput[k], testSecDeOutput[k]); //对本组的其他条纹进行解码
                        //cout << "\tthe sequence k " << k << " in outer check" << endl;
                        for (int j = 0; j < blocksize; j ++) {
                            testSecDeOutput[i][j] =  testSecDeOutput[i][j]  ^ testSecDeOutput[k][j];
                        }
                    }
                    flag[k] = 1;
                }
                for (int j = 0; j < blocksize; j ++) {
                    testSecDeOutput[i][j] = testSecDeOutput[i][j] ^ testXORDeOutput[Group][j];
                }
                if(testSecDeOutput[i] == testSecOutput[i]) {
                    //cout << "\tUsing the outer XOREncode_"<< i <<" success and the sequence after repair is:\t" << testSecDeOutput[i] <<endl;
                    //testSecDeInput[i] = testSecDeOutput[i]; //要记得这个条纹修复后，测试的时候，记得把DeInput给替换过来，不然会出现问题
                } else {
                    //cout << "\tRepair failed, testSecDeOutput is : " << testSecDeOutput[i] << endl;
                    //cout << "\tThere are more than two errors occurred in more than two sequences, repair failed.\t" << endl;
                }
            } else {
                //cout << "\tThe XOR have more than 3 errors, repair failed:\t " << testXORDeOutput[i / 4] << endl;
            }
        }
    }
}


/**
* 测试1位错误 所用时间
*/
void BCHCode::testOneError() {
    //Time_consuming_Read = 0;
    //4个条纹里生成1位错误
    cout << "$4:\ttest1: gengerate one error:\t" << endl;
    for(int n = 0; n < stripesize; n++) {
        testSecDeInput = testSecOutput;
        for(int errLoc1 = 0; errLoc1 < codeLen; errLoc1++) {
            testSecDeInput[n].flip(errLoc1);

            cout << endl;
            cout << "\tThe sequence_" << n << " after insert error " << "at " << errLoc1 <<" is:\t";
            cout << testSecDeInput[n] << endl;

            //实现读的计时
            gettimeofday(&start_time, NULL);
            ErrorCorrection();
            gettimeofday(&end_time, NULL);
            Time_consuming_Read += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
        }
    }
    cout << "\tFunction of gengerate one error and decode them execution time is " << (double)Time_consuming_Read  / 1000000 << " s" << endl;
}

/**
* 测试2位错误 所用时间
*/
void BCHCode::testTwoErrors() {
    //Time_consuming_Read = 0;
    //每个条纹里生成2位错误
    cout << "$5:\ttest2: gengerate two errors:\t" << endl;
    for(int n = 0; n < stripesize; n++) {
        for (int errLoc1 = 0; errLoc1 < codeLen - 1; errLoc1 ++) {
            for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen; errLoc2 ++) {
                testSecDeInput[n].flip(errLoc1);
                testSecDeInput[n].flip(errLoc2);

                cout << endl;
                cout << "\tThe sequence_" << n << " after insert errors " << "at " << errLoc1 << "," << errLoc2 <<" is:\t";
                cout << testSecDeInput[n] << endl;

                //实现读的计时
                gettimeofday(&start_time, NULL);
                ErrorCorrection();
                gettimeofday(&end_time, NULL);
                Time_consuming_Read += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
            }
        }
    }
    cout << "\tFunction of gengerate two errors and decode them execution time is " << (double)Time_consuming_Read  / 1000000 << " s" << endl;
}

/**
* 测试3位错误 所用时间
*/
void BCHCode::testThreeErrors() {
    //Time_consuming_Read = 0;
    //每个条纹里生成3位错误
    cout << "$6:\ttest3: gengerate three errors:\t" << endl;
    for(int n = 0; n < stripesize; n++) {
        for (int errLoc1 = 0; errLoc1 < codeLen - 2; errLoc1 ++) {
            for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen - 1; errLoc2 ++) {
                for(int errLoc3 = errLoc2 + 1; errLoc3 < codeLen; errLoc3 ++) {
                    testSecDeInput[n].flip(errLoc1);
                    testSecDeInput[n].flip(errLoc2);
                    testSecDeInput[n].flip(errLoc3);

                    cout << endl;
                    cout << "\tThe sequence_" << n << " after insert errors " << "at " << errLoc1 << "," << errLoc2 << "," << errLoc3<<" is:\t";
                    cout << testSecDeInput[n] << endl;

                    //实现读的计时
                    gettimeofday(&start_time, NULL);
                    ErrorCorrection();
                    gettimeofday(&end_time, NULL);
                    Time_consuming_Read += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
                }
            }
        }
    }//服务器里跑了5.02S 4.82s 4.79s 4.78s
    cout << "\tFunction of gengerate three errors and decode them execution time is " << (double)Time_consuming_Read  / 1000000 << " s" << endl;

}

void BCHCode::testFunctional() {
    //testWrite(); //生成编码
    //testOneError();  //测试1位错误 所用时 函数
    //testTwoErrors();  //测试2位错误 所用时 函数
    //testThreeErrors(); //测试3位错误 所用时 函数
}


/**
* 测试读 所用时间
*/
void BCHCode::testRead() {
    //每个条纹里随机生成[0,3]个错误 stripesize = 4
    for(int num = 0; num < number_Group; num++) {
        for(int i = 0; i < stripesize; i++) {
            int m = 0, n = 3;
            int temp = rand()%(n - m + 1) + m; //生成[m, n]之间的随机数 temp为testSecDeInput错误的个数
            for(int j = 0; j < temp; j++) {
                int errLoc = rand()%(43 - 0 + 1) + 0; //随机生成[0,43]的错误位置
                //testSecDeInput[num * 4 + i].flip(errLoc); //产生错误的时候，暂时测的是，如果是3个错误 0 1 2错误 如果是2个错误 0 1出错
                testSecDeInput[num * 4 + i].flip(errLoc); //产生错误的时候，暂时测的是，如果是3个错误 0 1 2错误 如果是2个错误 0 1出错
            }
            //cout << "\tThe sequence_" << num * 4 + i << " after insert "<< temp <<" error(s) " << " is:\t"  << testSecDeInput[num * 4 + i] << endl;
        }
    }


    //实现读的计时
    gettimeofday(&start_time, NULL);
    ErrorCorrection();
    gettimeofday(&end_time, NULL);
    Time_consuming_Read += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
    //cout << "\tFunction Read execution time already is  " << (double)Time_consuming_Read  / 1000000 << " s" << endl;
    //cout << endl;
}

/**
*   测试写
*/
void BCHCode::testWrite() {

    //验证完之后 记得重置 防止后一组的实验对前一组产生影响
    int tmp = 0;
    for(int j = 0; j < testSecInput.size(); j++) { //生成4个32位的随机序列
        for (int i = 0; i < testSecInput[j].size(); i ++) {
            tmp = rand() % 2; //产生随机值
            if (tmp) {
                testSecInput[j].set(i);
            }
        }
        //cout << "\tBCH_"<< j << " encoding before:\t";
        //cout << testSecInput[j] << endl;
        //实现写的计时 其中写包括 随机生成39位信息位 + 计算校验位 + 生成的异或
        gettimeofday(&start_time, NULL);
        BCHEncode(testSecInput[j], testSecOutput[j]);
        gettimeofday(&end_time, NULL);
        Time_consuming_Write += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
        //cout << "\tBCH_"<< j << " encoding after:\t";
        //cout << testSecOutput[j] << endl;
        //每生成一个编码条纹，做一次校验
        int Group = j / stripesize;
        for (int i = 0; i < blocksize; i ++) {
            if(testSecOutput[j][i]) {
                testXOR[Group].flip(i);
            }
        }
    }

//        for(int i = 0; i < testXOR.size(); i++){
//            cout << "\ttestXOR_" << i << " is: \t\t\t" << testXOR[i] << endl;
//        }

    testSecDeInput = testSecOutput;
    testXORDeInput = testXOR;
    //cout << "\tFunction Write execution time already is  " << (double)Time_consuming_Write  / 1000000 << " s" << endl << endl;

}



/**
 BCH编码测试程序
 */
void BCHCode::testBCH() {
    //cout << "$1:\tsetTable(make table and saved errTableBCH_44_32.txt)" << endl;
    setTable();
    //cout << "$2:\tgetTable(read from errTableBCH_44_32.txt and saved errTable arrays)" << endl;
    getTable();

    //testFunctional(); //功能性测试

    Time_consuming_Read = 0;
    Time_consuming_Write = 0;
    srand((unsigned int)time(NULL)); //srand()一定要放在循环外面或者是循环调用的外面，否则的话得到的是相同的随机数
    //for(int num = 0; num < number_strip; num++) {
    //cout << "#" << num + 1 << ":\tgenerate random numbers:\t" << endl;
    testWrite(); //测试写用时 目前包括 生成39位随机数的值 + BCH32位信息码 变为 44位信息码的过程 + XOR的过程
    testRead();  //测试读 所用时 包括校验的过程函数
    //}
    cout << "\tWrite operation time is " << (double)Time_consuming_Write / 1000000 << " s" << endl;
    cout << "\tRead operation time is " << (double)Time_consuming_Read / 1000000 << " s" << endl;
    cout << "\tTotal operation Time is " << (double)Time_consuming_Write / 1000000 +  (double)Time_consuming_Read / 1000000 << " s" << endl;
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
void BCHCode::errTest() {

    cout << "$1:\tsetTable(制作错误位置表，保存到errTable.txt)" << endl;
    setTable();
    cout << "$2:\tgetTable(从errTableBCH_44_32.txt读去错误模式，并倒排保存到errTable数组中，下标为伴随式，值为错误位置)" << endl;
    getTable();

    cout << "$3:\terrTest(遍历3个错误的情况，得到新旧校验异或，并查询errTable，如果存在，则有冲突)" << endl;
    for (int errLoc1 = 0; errLoc1 < codeLen - 2; errLoc1 ++) {
        for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen - 1; errLoc2 ++) {
            for (int errLoc3 = errLoc2 + 1; errLoc3 < codeLen; errLoc3 ++) {
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

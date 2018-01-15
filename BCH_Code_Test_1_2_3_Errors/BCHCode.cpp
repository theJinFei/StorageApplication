#include "BCHCode.h"
#include <time.h>
#include <cstdlib>

bitset<dataLen> enInput("00001100000000000000000000000000");
bitset<codeLen> enOutput;
bitset<codeLen> deInput;
bitset<codeLen> deOutput;
bitset<parityLen> parityXOR;

vector<bitset<dataLen> > testSecInput(4, bitset<dataLen> (0)); //输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecOutput(4, bitset<codeLen> (0)); //输出 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeInput(4, bitset<codeLen> (0)); //进行编码 输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeOutput(4, bitset<codeLen> (0)); //进行编码 输出 以四个编码条纹为一组 进行第二级的纠错
bitset<codeLen> testXOR;

//vector<long long> errTable(4096, -1);
unordered_map<bitset<parityLen>, bitset<codeLen>> errTable;

//  BCH(44, 32)生成矩阵 没有加单位矩阵的
vector<vector<int>> generateMatrix = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,1,0, 1,0,0,1, 1,1,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,1,0,1,0,1,0,0,1,1,1,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,0,1,0,1,0,0,1,1,1},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,1,1,1,1,0,0,1,1,1,1},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,1,1,1,0,1,1,1,1,0,1,1},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,1,0,1,0,0,1,0,0,0,0,1},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,1,0,0,0,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,1,1,0,0,0,0,0,0,1,1,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,1,0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,1,1,0,0,0,1,1,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,1,1,1,0,0,0,1,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,1,1,1,1,0,0,0,1,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,0,1,0,1,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,1,0,0,1,0,1,0,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,0,0,1,0,1,0,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,1,0,0,1,0,1,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,1,0,0,0,0,0,1,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,1,1,1,1,1,0,0,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,1,1,1,1,0,0,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,1,1,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0, 1,0,1,0,0,1,1,0,0,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0, 1,1,1,1,1,0,1,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0, 0,1,1,1,1,1,0,1,0,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0, 1,0,0,1,0,1,1,1,0,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0, 1,1,1,0,0,0,1,0,0,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0, 1,1,0,1,1,0,0,0,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0, 1,1,0,0,0,1,0,1,1,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0, 1,1,0,0,1,0,1,1,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0, 1,1,0,0,1,1,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0, 0,1,1,0,0,1,1,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, 0,0,1,1,0,0,1,1,0,0,0,1}
};

//  BCH(44, 32)伴随矩阵，其实就是新旧校验位的异或值
const vector<vector<int>> adjointMatrix = {
    {1,0,0,1,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0, 1,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,1,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0, 0,1,0,0,0,0,0,0,0,0,0,0},
    {1,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1, 0,0,1,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1, 0,0,0,1,0,0,0,0,0,0,0,0},
    {1,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,0,1,0,1,1,0,0, 0,0,0,0,1,0,0,0,0,0,0,0},
    {0,1,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,0,1,0,1,1,0, 0,0,0,0,0,1,0,0,0,0,0,0},
    {0,0,1,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,0,1,0,1,1, 0,0,0,0,0,0,1,0,0,0,0,0},
    {1,0,0,0,1,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,1,0,0,1, 0,0,0,0,0,0,0,1,0,0,0,0},
    {1,1,0,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,1,0,0,0},
    {1,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,0,0, 0,0,0,0,0,0,0,0,0,1,0,0},
    {0,1,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,0, 0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1, 0,0,0,0,0,0,0,0,0,0,0,1}
};

int adjointMatrix_T[44][12]; //存放校验矩阵的转置


/**
 BCH编码

 @param input 输入32位信息位
 @param output 输出44位编码后的BCH码
 */
void BCHCode::BCHEncode(bitset<dataLen> & input, bitset<codeLen> & output) {
    //printf("BCHEncode start...\n");
    for(int j = 0; j < codeLen; j++) {
        output[codeLen - 1 - j] = 0;
        for(int i = 0; i < dataLen; i++) {
            if(input[dataLen - 1 - i] * generateMatrix[i][j]) {
                output.flip(codeLen - 1 - j);
            }
        }
    }
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
 @param output 错误伴随式
 */
void BCHCode::getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output) {
    //printf("getParityXOR start...\n");
    for(int i  = 0; i < parityLen; i++) {
        output[i] = 0;
        for(int j = 0; j < codeLen; j++) {
            if(input[codeLen - 1  - j] * adjointMatrix[i][j]) {
                output.flip(i);
            }
        }
    }
    //cout << "\tgenerate 7 parithy number is:\t" << output << endl;
    //printf("getParityXOR end...\n");
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
    cout << enInput << endl;
    BCHEncode(enInput, enOutput);
    cout << enOutput << endl;

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
void BCHCode::testOneorTwoError() {


    //先执行这个循环，将只发生1位或2位错误编码的情况 给纠错出来
    for(int i = 0; i < testSecDeInput.size(); i++) {
        BCHDecode(testSecDeInput[i], testSecDeOutput[i]);
    }
    for(int i = 0; i < testSecDeOutput.size(); i++) {
        if(testSecDeOutput[i] == testSecOutput[i]) {
            cout << "\tUsing the inner BCHEncode_" << i <<" success and the sequence after repair is: \t" << testSecDeOutput[i] <<endl;
            testSecDeInput[i] = testSecOutput[i];
        } else {
            //如果只有1个序列其中的2位以上发生了错误，则抛给异或进行解码
            cout << "\tThe sequence:\t\t" << testSecDeOutput[i] << "\thave more than 2 errors, enter the second level of redundancy check:\t" << endl;
            //做异或校验
            for (int j = 0; j < blocksize; j ++) {
                int temp = 0;
                for(int k = 0; k < testSecDeOutput.size(); k++) {
                    //cout << "\ttestSecOutput:\t" << testSecOutput[k] << endl;
                    if(k != i && testSecDeOutput[k][j]) {
                        temp++;
                        //cout << "\tk:\t" << j << "\tj:\t" << j << testSecDeOutput[k][j] << endl;
                    } else {
                        continue;
                    }
                }
                if(testXOR[j]) {
                    temp++;
                }
                //cout << " temp " << temp << endl;
                if(temp % 2 != 0) {
                    testSecDeOutput[i][j] = 1;
                } else {
                    testSecDeOutput[i][j] = 0;
                }
            }
            if(testSecDeOutput[i] == testSecOutput[i]) {
                cout << "\tUsing the outer XOREncode_"<< i <<" success and the sequence after repair is:\t" << testSecDeOutput[i] <<endl;
                testSecDeInput[i] = testSecOutput[i]; //要记得这个条纹修复后，测试的时候，记得把DeInput给替换过来，不然会出现问题
            } else {
                cout << "\tThere are more than two errors occurred in more than two sequences, repair failed.\t" << endl;
            }
        }
    }

}

/**
 BCH编码测试程序
 */
void BCHCode::testBCH() {
    cout << "$1:\tsetTable(make table and saved errTableBCH_44_32.txt)" << endl;
    setTable();
    cout << "$2:\tgetTable(read from errTableBCH_44_32.txt and saved errTable arrays)" << endl;
    getTable();

    cout << "$3:\tgenerate random numbers:\t" << endl;
    srand((unsigned int)time(NULL));
    int tmp = 0;
    enInput.reset();
    for(int j = 0; j < testSecInput.size(); j++) { //生成4个32位的随机序列
        for (int i = 0; i < testSecInput[j].size(); i ++) {
            tmp = rand() % 2;
            if (tmp) {
                //enInput.set(i);
                testSecInput[j].set(i);
            }
        }
        cout << "\tBCH_"<< j << " encoding before:\t";
        cout << testSecInput[j] << endl;
        BCHEncode(testSecInput[j], testSecOutput[j]);
        cout << "\tBCH_"<< j << " encoding after:\t";
        cout << testSecOutput[j] << endl;
    }
    //实行偶校验
    for (int i = 0; i < blocksize; i ++) {
        int temp = 0;
        for(int j = 0; j < testSecOutput.size(); j++) {
            if(testSecOutput[j][i]) {
                temp++;
            }
        }
        if(temp % 2 == 0) {
            testXOR[i] = 0;
        } else {
            testXOR[i] = 1;
        }
    }
    cout << "\tXOR result is : \t";
    cout << testXOR << endl;

    bool isEnd = 0;
    int errNum = 0;
    while(!isEnd) {
        if(isEnd) {
            break;
        }
        testSecDeInput = testSecOutput;
        double second1 = time((time_t*)NULL);
        //4个条纹里生成1位错误
        cout << "$4:\ttest1: gengerate one error:\t" << endl;
        for(int n = 0; n < stripesize; n++) {
            //testSecDeInput = testSecOutput;
            for(int errLoc1 = 0; errLoc1 < codeLen; errLoc1++) {
                testSecDeInput[n].flip(errLoc1);
                cout << "\tThe sequence_" << n << " after insert error " << "at " << errLoc1 <<" is:\t";
                cout << testSecDeInput[n] << endl;
                testOneorTwoError();
            }
        }

        //每个条纹里生成2位错误
//        cout << "$5:\ttest2: gengerate two errors:\t" << endl;
//        for(int n = 0; n < stripesize; n++) {
//            for (int errLoc1 = 0; errLoc1 < codeLen - 1; errLoc1 ++) {
//                for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen; errLoc2 ++) {
//                    testSecDeInput[n].flip(errLoc1);
//                    testSecDeInput[n].flip(errLoc2);
//                    cout << "\tThe sequence_" << n << " after insert errors " << "at " << errLoc1 << "," << errLoc2 <<" is:\t";
//                    cout << testSecDeInput[n] << endl;
//                    //testSecDeInput[n] = testSecOutput[n];
//                    testOneorTwoError();
//                }
//            }
//        }
        //每个条纹里生成3位错误
//        cout << "$6:\ttest3: gengerate three errors:\t" << endl;
//        for(int n = 0; n < stripesize; n++) {
//            for (int errLoc1 = 0; errLoc1 < codeLen - 2; errLoc1 ++) {
//                for (int errLoc2 = errLoc1 + 1; errLoc2 < codeLen - 1; errLoc2 ++) {
//                    for(int errLoc3 = errLoc2 + 1; errLoc3 < codeLen; errLoc3 ++){
//                        testSecDeInput[n].flip(errLoc1);
//                        testSecDeInput[n].flip(errLoc2);
//                        testSecDeInput[n].flip(errLoc3);
//                        cout << "\tThe sequence_" << n << " after insert errors " << "at " << errLoc1 << "," << errLoc2 << "," << errLoc3<<" is:\t";
//                        cout << testSecDeInput[n] << endl;
//                        testSecDeInput[n] = testSecOutput[n];
//                        testOneorTwoError();
//                    }
//                }
//            }
//        }
        double second2 = time((time_t*)NULL);
        cout << "\tFunction execution time is " << second2 - second1 << " s" << endl;
        cout << "\tExit the test? yes : 1 , no : 0\t";
        cin >> isEnd;
    }
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

#include "BCHCode.h"
#include <time.h>
#include <cstdlib>

bitset<dataLen> enInput("00001100000000000000000000000000");
bitset<codeLen> enOutput;
bitset<codeLen> deInput;
bitset<codeLen> deOutput;
bitset<parityLen> parityXOR;

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
//    for (int i = 0; i < generateMatrix.size(); i++) {
//        output[i] = 0;
//        for (int j = 0; j < generateMatrix[i].size(); j++) {
//            //output[i] ^= input[generateMatrix[i][j]];
////            if (input.test(generateMatrix[i][j])) { //在b.test(pos) b在pos处的二进制位是否为1？
////                output.flip(i);
////            }
//
//        }
//    }

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
        cout << "\t错误过多，无法修复" << endl;
    }
}


/**
 求错误伴随式

 @param input 接收到的数据
 @param output 错误伴随式
 */
void BCHCode::getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output) {
    //printf("getParityXOR start...\n");
//    for (int i = 0; i < adjointMatrix.size(); i++) {
//        output[i] = 0;
//        for (int j = 0; j < adjointMatrix[i].size(); j++) {
//            //output[i] ^= input[adjointMatrix[i][j]];
//            if (input.test(adjointMatrix[i][j])) {
//                output.flip(i);
//            }
//        }
//    }

    for(int i  = 0; i < parityLen; i++){
        output[i] = 0;
        for(int j = 0; j < codeLen; j++){
            if(input[codeLen - 1  - j] * adjointMatrix[i][j]){
                output.flip(i);
            }
        }
    }
    cout << "\tgenerate 7 parithy number is:\t" << output << endl;
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
 BCH编码测试程序
 */
void BCHCode::testBCH() {
    cout << "$1:\tsetTable(make table and saved errTableBCH_44_32.txt)" << endl;
    setTable();
    cout << "$2:\tgetTable(read from errTableBCH_44_32.txt and saved errTable arrays)" << endl;
    getTable();

    cout << "$3:\tgenerate random numbers:\t";
    srand((unsigned int)time(NULL));
    int tmp = 0;
    enInput.reset();
    for (int i = 0; i < dataLen; i ++) {
        tmp = rand() % 2;
        if (tmp) {
            enInput.set(i);
        }
    }
    cout << enInput << endl;
    BCHEncode(enInput, enOutput);
    cout << "\tBCH encoded:\t\t\t";
    cout << enOutput << endl;

    bool notEnd = 1;
    while (notEnd) {
        cout << "$4:\tinput one or two mistake position:\t";
        vector<int> errLoc;
        int tmpLoc = 0;
        while(cin >> tmpLoc) {
            errLoc.push_back(tmpLoc);
            if(cin.get() == '\n')
                break;
        }
        deInput = enOutput;
        for (int i = 0; i < errLoc.size(); i ++) {
            deInput.flip(errLoc[i]);
        }
        cout << "\tchange the number is:\t\t" << deInput << endl;
        BCHDecode(deInput, deOutput);
        cout << "\tafter repair :\t\t\t" << deOutput << endl;
        if (deOutput == enOutput) {
            cout << "\trepair successfully" << endl;

        } else {
            cout << "\trepair failed" << endl;
        }
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

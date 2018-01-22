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

vector<bitset<dataLen> > testSecInput(4, bitset<dataLen> (0)); //输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecOutput(4, bitset<codeLen> (0)); //输出 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeInput(4, bitset<codeLen> (0)); //进行编码 输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeOutput(4, bitset<codeLen> (0)); //进行编码 输出 以四个编码条纹为一组 进行第二级的纠错
bitset<codeLen> testXOR("00000000000000000000000000000000000000000000");
bitset<codeLen> testXORDeInput("00000000000000000000000000000000000000000000");
bitset<codeLen> testXORDeOutput("00000000000000000000000000000000000000000000");

//vector<long long> errTable(4096, -1);
unordered_map<bitset<parityLen>, bitset<codeLen>> errTable;

unsigned long number_strip = 50000; //设置的几组数据 1GB数据 大概是1GB的数据量
int flag[4] = {0};

struct  timeval  start_time;
struct  timeval  end_time;
unsigned long Time_consuming_Read = 0, Time_consuming_Write = 0;

void fun_GenerateParity(bitset<dataLen> & input, bitset<codeLen> & output)
{
    //进行赋值操作
    for(int i = 0; i < dataLen; i++){
        output[i + 12] = input[i];
    }
    output[11] = input[31] ^ input[28] ^ input[27] ^ input[26] ^ input[25] ^ input[22] ^ input[21] ^ input[19] ^ input[15] ^ input[14] ^ input[10] ^ input[9] ^ input[7] ^ input[6] ^ input[5] ^ input[4] ^ input[3] ^ input[2];
    output[10] = input[30] ^ input[27] ^ input[26] ^ input[25] ^ input[24] ^ input[21] ^ input[20] ^ input[18]  ^ input[14] ^ input[13] ^ input[9] ^ input[8] ^ input[6] ^ input[5] ^ input[4] ^ input[3] ^ input[2] ^ input[1];
    output[9] = input[31] ^ input[29] ^ input[28]  ^ input[27] ^ input[24] ^ input[23] ^ input[22] ^ input[21] ^ input[20] ^ input[17] ^ input[15] ^ input[14] ^ input[13]  ^ input[12] ^ input[10] ^ input[9] ^ input[8] ^ input[6]  ^ input[1] ^ input[0];
    output[8] = input[30] ^ input[28] ^ input[27] ^ input[26] ^ input[23] ^ input[22] ^ input[21] ^ input[20] ^ input[19] ^ input[16] ^ input[14] ^ input[13] ^ input[12] ^ input[11]  ^ input[9] ^ input[8] ^ input[7] ^ input[5] ^ input[0];
    output[7] = input[31] ^ input[29] ^ input[28] ^ input[20] ^ input[18] ^ input[14] ^ input[13] ^ input[12] ^ input[11] ^ input[9] ^ input[8] ^ input[5] ^ input[3] ^ input[2];
    output[6] = input[30] ^ input[28] ^ input[27] ^ input[19] ^ input[17] ^ input[13]^ input[12] ^ input[11] ^ input[10] ^ input[8] ^ input[7] ^ input[4] ^ input[2] ^ input[1];
    output[5] = input[29] ^ input[27] ^ input[26] ^ input[18] ^ input[16] ^ input[12] ^ input[11] ^ input[10] ^ input[9] ^ input[7] ^ input[6] ^ input[3] ^ input[1] ^ input[0];
    output[4] = input[31] ^ input[27]  ^ input[22] ^ input[21] ^ input[19] ^ input[17] ^ input[14] ^ input[11] ^ input[8] ^ input[7] ^ input[4] ^ input[3] ^ input[0];
    output[3] = input[31] ^ input[30] ^ input[28] ^ input[27] ^ input[25] ^ input[22] ^ input[20] ^ input[19] ^ input[18] ^ input[16] ^ input[15] ^ input[14] ^ input[13] ^ input[9] ^ input[5] ^ input[4];
    output[2] = input[31] ^ input[30] ^ input[29] ^ input[28] ^ input[25] ^ input[24] ^ input[22] ^ input[18] ^ input[17] ^ input[13] ^ input[12] ^ input[10] ^ input[9] ^ input[8] ^ input[7] ^ input[6] ^ input[5] ^ input[2];
    output[1] = input[30] ^ input[29] ^ input[28] ^ input[27] ^ input[24] ^ input[23] ^ input[21] ^ input[17] ^ input[16] ^ input[12] ^ input[11] ^ input[9] ^ input[8] ^ input[7] ^ input[6] ^ input[5] ^ input[4] ^ input[1];
    output[0] = input[29] ^ input[28] ^ input[27] ^ input[26] ^ input[23] ^ input[22] ^ input[20] ^ input[16] ^ input[15] ^ input[11] ^ input[10] ^ input[8] ^ input[7] ^ input[6] ^ input[5] ^ input[4] ^ input[3] ^ input[0];
}

void fun_Adjoint(bitset<codeLen> & input, bitset<parityLen> & output)
{
    output[0] = input[43] ^ input[40] ^ input[39] ^ input[38] ^ input[37] ^ input[34] ^ input[33] ^ input[31] ^ input[27] ^ input[26] ^ input[22] ^ input[21] ^ input[19] ^ input[18] ^ input[17] ^ input[16] ^input[15] ^ input[14] ^ input[11];
	output[1] = input[42] ^ input[39] ^ input[38] ^ input[37] ^ input[36] ^ input[33] ^ input[32] ^ input[30] ^ input[26] ^ input[25] ^ input[21] ^ input[20] ^ input[18] ^ input[17] ^ input[16] ^ input[15] ^ input[14] ^ input[13] ^ input[10];
	output[2] = input[43] ^ input[41] ^ input[40] ^ input[39] ^ input[36] ^ input[35] ^ input[34] ^ input[33] ^ input[32] ^ input[29] ^ input[27] ^ input[26] ^ input[25] ^ input[24] ^ input[22] ^ input[21] ^ input[20] ^ input[18] ^ input[13] ^ input[12] ^ input[9] ;
	output[3] = input[42] ^ input[40] ^ input[39] ^ input[38] ^ input[35] ^ input[34] ^ input[33] ^ input[32] ^ input[31] ^ input[28] ^ input[26] ^ input[25] ^ input[24] ^ input[23] ^ input[21] ^ input[20] ^ input[19] ^ input[17] ^ input[12] ^ input[8];
	output[4] = input[43] ^ input[41] ^ input[40] ^ input[32] ^ input[30] ^ input[26] ^ input[25] ^ input[24] ^ input[23] ^ input[21] ^ input[20] ^ input[17] ^ input[15] ^ input[14] ^ input[7];
	output[5] = input[42] ^ input[40] ^ input[39] ^ input[31] ^ input[29] ^ input[25] ^ input[24] ^ input[23] ^ input[22] ^ input[20] ^ input[19] ^ input[16] ^ input[14] ^ input[13] ^ input[6];
	output[6] = input[41] ^ input[39] ^ input[38] ^ input[30] ^ input[28] ^ input[24] ^ input[23] ^ input[22] ^ input[21] ^ input[19] ^ input[18] ^ input[15] ^ input[13] ^ input[12] ^ input[5];
	output[7] = input[43] ^ input[39] ^ input[34] ^ input[33] ^ input[31] ^ input[29] ^ input[26] ^ input[23] ^ input[20] ^ input[19] ^ input[16] ^ input[15] ^ input[12] ^ input[4];
	output[8] = input[43] ^ input[42] ^ input[40] ^ input[39] ^ input[37] ^ input[34] ^ input[32] ^ input[31] ^ input[30] ^ input[28] ^ input[27] ^ input[26] ^ input[25] ^ input[21] ^ input[17] ^ input[16] ^ input[3];
	output[9] = input[43] ^ input[42] ^ input[41] ^ input[40] ^ input[37] ^ input[36] ^ input[34] ^ input[30] ^ input[29] ^ input[25] ^ input[24] ^ input[22] ^ input[21] ^ input[20] ^ input[19] ^ input[18] ^ input[17] ^ input[14] ^ input[2];
	output[10] = input[42] ^ input[41] ^ input[40] ^ input[39] ^ input[36] ^ input[35] ^ input[33] ^ input[29] ^ input[28] ^ input[24] ^ input[23] ^ input[21] ^ input[20] ^ input[19] ^ input[18] ^ input[17] ^ input[16] ^ input[13] ^ input[1];
	output[11] = input[41] ^ input[40] ^ input[39] ^ input[38] ^ input[35] ^ input[34] ^ input[32] ^ input[28] ^ input[27] ^ input[23] ^ input[22] ^ input[20] ^ input[19] ^ input[18] ^ input[17] ^ input[16] ^ input[15] ^ input[12] ^ input[0];
}

//  BCH(44, 32)生成矩阵 单位矩阵的
vector<vector<int>> generateMatrix = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 1,0,1,0,1,0,0,1,1,1,0,0},
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



/**
 BCH编码

 @param input 输入32位信息位
 @param output 输出44位编码后的BCH码
 */
void BCHCode::BCHEncode(bitset<dataLen> & input, bitset<codeLen> & output) {
    //printf("BCHEncode start...\n");
//    for(int j = 0; j < codeLen; j++) {
//        output[codeLen - 1 - j] = 0;
//        for(int i = 0; i < dataLen; i++) {
//            if(input[dataLen - 1 - i] * generateMatrix[i][j]) {
//                output.flip(codeLen - 1 - j);
//            }
//        }
//    }
    fun_GenerateParity(input, output);
    //printf("BCHEncode end...\n");
}


void BCHCode::BCHDecode(bitset<codeLen> & input, bitset<codeLen> & output) {
    //getParityXOR(input, parityXOR);
    fun_Adjoint(input, parityXOR);
//    cout << "\tThe result of parityXOR is " << parityXOR << endl;
//    fun_Adjoint(input, parityXOR);
//    cout << "\tThe result of Adjoint is : " << parityXOR << endl;
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
//    for(int i  = 0; i < parityLen; i++) {
//        output[i] = 0;
//        for(int j = 0; j < codeLen; j++) {
//            if(input[codeLen - 1  - j] * adjointMatrix[i][j]) {
//                output.flip(i);
//            }
//        }
//    }


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
    //cout << enInput << endl;
    BCHEncode(enInput, enOutput);
    //cout << enOutput << endl;

    //出现1位错误，错误位置位errLoc
    for(int errLoc = 0; errLoc < codeLen; errLoc ++) {
        //memcpy(deInput, enOutput, codeLen * sizeof(int));
        deInput = enOutput;
        deInput.flip(errLoc);             //翻转第errLoc位

        //getParityXOR(deInput, parityXOR);
        fun_Adjoint(deInput, parityXOR);

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

            //getParityXOR(deInput, parityXOR);
            fun_Adjoint(deInput, parityXOR);

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
    memset(flag, 0, sizeof(flag)); //flag作为刚开始是否无错误的标志，如果刚开始就无错的话，就不用纠正了
    for(int i = 0; i < testSecDeInput.size(); i++) {
        if(testSecDeInput[i] == testSecOutput[i]) { //如果传过来的本身 没有错误
            cout << "\tThe sequence_"<< i << " " << testSecDeInput[i] <<  " has no errors" <<endl;
            testSecDeOutput[i] = testSecDeInput[i];
            flag[i] = 1;
        } else {
            BCHDecode(testSecDeInput[i], testSecDeOutput[i]);
        }
    }


    for(int i = 0; i < testSecDeOutput.size(); i++) {
        if(testSecDeOutput[i] == testSecOutput[i] && !flag[i]) {
            cout << "\tUsing the inner BCHEncode_" << i <<" success and the sequence after repair is: \t" << testSecDeOutput[i] <<endl;
            testSecDeInput[i] = testSecOutput[i];
        } else if(testSecDeOutput[i] != testSecOutput[i]) {
            //如果只有1个序列其中的2位以上发生了错误，则抛给异或进行解码
            cout << "\tThe sequence_"<< i << " " << testSecDeOutput[i] << " have more than 2 errors, enter the second level of redundancy check:\t" << endl;
            BCHDecode(testXORDeInput, testXORDeOutput); //对XOR进行解码
            if(testXORDeOutput == testXOR) { //证明XOR无错误
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
                    if(temp % 2 != 0) {
                        testSecDeOutput[i][j] = 1;
                    } else {
                        testSecDeOutput[i][j] = 0;
                    }
                }
                if(testSecDeOutput[i] == testSecOutput[i]) {
                    cout << "\tUsing the outer XOREncode_"<< i <<" success and the sequence after repair is:\t" << testSecDeOutput[i] <<endl;
                    testSecDeInput[i] = testSecDeOutput[i]; //要记得这个条纹修复后，测试的时候，记得把DeInput给替换过来，不然会出现问题
                } else {
                    //cout << "\t" << testSecDeOutput[i] << endl;
                    cout << "\tThere are more than two errors occurred in more than two sequences, repair failed.\t" << endl;
                }
            }
            else {
                cout << "\tThe XOR have more than 3 errors, repair failed:\t " << testXORDeOutput << endl;
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
    }  //146.934s 159.525s      0.31s 0.32s 0.32s
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
    testWrite(); //生成编码
    testOneError();  //测试1位错误 所用时 函数
    //testTwoErrors();  //测试2位错误 所用时 函数
    //testThreeErrors(); //测试3位错误 所用时 函数
}


/**
* 测试读 所用时间
*/
void BCHCode::testRead() {
    //每个条纹里随机生成[0,3]个错误
    for(int i = 0; i < stripesize; i++) {
        int m = 0, n = 3;
        int temp = rand()%(n - m + 1) + m; //生成[m, n]之间的随机数 temp为testSecDeInput错误的个数
        //cout << "\terrLoc at";
        for(int j = 0; j < temp; j++) {
            int errLoc = rand()%(43 - 0 + 1) + 0; //随机生成[0,43]的错误位置
            testSecDeInput[i].flip(errLoc); //产生错误的时候，暂时测的是，如果是3个错误 0 1 2错误 如果是2个错误 0 1出错
            //cout << errLoc << "," ;
        }
        //cout << endl;
        cout << "\tThe sequence_" << i << " after insert "<< temp <<" error(s) " << " is:\t"  << testSecDeInput[i] << endl;
    }

    //实现读的计时
    gettimeofday(&start_time, NULL);
    ErrorCorrection();
    gettimeofday(&end_time, NULL);
    Time_consuming_Read += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
    cout << "\tFunction Read execution time already is  " << (double)Time_consuming_Read  / 1000000 << " s" << endl;
    cout << endl;
}

/**
*   测试写
*/
void BCHCode::testWrite() {

    //验证完之后 记得重置 防止后一组的实验对前一组产生影响
    testXOR.reset();
    for(int i = 0; i < testSecInput.size(); i++) {
        testSecDeOutput[i].reset();
        testSecDeInput[i].reset();
        testSecOutput[i].reset();
        testSecInput[i].reset(); //必不可少 把第i个条纹置为0，防止上一组的数据产生影响
    }

    int tmp = 0;
    for(int j = 0; j < testSecInput.size(); j++) { //生成4个32位的随机序列
        //实现写的计时 其中写包括 随机生成39位信息位 + 计算校验位 + 生成的异或
        gettimeofday(&start_time, NULL);
        for (int i = 0; i < testSecInput[j].size(); i ++) {
            tmp = rand() % 2; //产生随机值
            if (tmp) {
                testSecInput[j].set(i);
            }
        }
        //cout << "\tBCH_"<< j << " encoding before:\t";
        //cout << testSecInput[j] << endl;
        //BCHEncode(testSecInput[j], testSecOutput[j]);
        //cout << "\tBCH_"<< j << " encoding after:\t";
        //cout << testSecOutput[j] << endl;
        fun_GenerateParity(testSecInput[j], testSecOutput[j]);
        cout << "\tfunGenerateParity after:" << testSecOutput[j] << endl;
        //每生成一个编码条纹，做一次校验
        for (int i = 0; i < blocksize; i ++) {
            if(testSecOutput[j][i]) {
                testXOR.flip(i);
            }
        }
        gettimeofday(&end_time, NULL);
        Time_consuming_Write += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
    }

    cout << "\tXOR result is : \t";
    cout << testXOR << endl;
    testSecDeInput = testSecOutput;
    testXORDeInput = testXOR;
    cout << "\tFunction Write execution time already is  " << (double)Time_consuming_Write  / 1000000 << " s" << endl << endl;

}



/**
 BCH编码测试程序
 */
void BCHCode::testBCH() {
    cout << "$1:\tsetTable(make table and saved errTableBCH_44_32.txt)" << endl;
    setTable();
    cout << "$2:\tgetTable(read from errTableBCH_44_32.txt and saved errTable arrays)" << endl;
    getTable();

    //testFunctional(); //功能性测试

    Time_consuming_Read = 0;
    Time_consuming_Write = 0;
    srand((unsigned int)time(NULL)); //srand()一定要放在循环外面或者是循环调用的外面，否则的话得到的是相同的随机数
    for(int num = 0; num < number_strip; num++) {
        cout << "#" << num + 1 << ":\tgenerate random numbers:\t" << endl;
        testWrite(); //测试写用时 目前包括 生成39位随机数的值 + BCH32位信息码 变为 44位信息码的过程 + XOR的过程
        testRead();  //测试读 所用时 包括校验的过程函数
    }
    cout << "\tWrite operation time is " << (double)Time_consuming_Write / 1000000 << " s" << endl;
    cout << "\tRead operation time is " << (double)Time_consuming_Read / 1000000 << " s" << endl;
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

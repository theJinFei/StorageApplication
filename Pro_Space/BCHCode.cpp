#include "BCHCode.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <bitset>
#include <time.h>
#include "main.h"

using namespace  std;
short Array_Table[4096] = { 0 };								//读表的数据结构，下标为对应的伴随式（12-bit），值为相应的错误位置

vector<vector<int>> adjointMatrix = {
	{ 0, 12, 15, 16, 17, 18, 21, 22, 24, 28, 29, 33, 34, 36, 37, 38, 39, 40, 41 },                            //a0
	{ 1, 13, 16, 17, 18, 19, 22, 23, 25, 29, 30, 34, 35, 37, 38, 39, 40, 41, 42 },                            //a1
	{ 2, 12, 14, 15, 16, 19, 20, 21, 22, 23, 26, 28, 29, 30, 31, 33, 34, 35, 37, 42, 43 },                    //a2
	{ 3, 13, 15, 16, 17, 20, 21, 22, 23, 24, 27, 29, 30, 31, 32, 34, 35, 36, 38, 43 },                        //a3
	{ 4, 12, 14, 15, 23, 25, 29, 30, 31, 32, 34, 35, 38, 40, 41 },                                            //a4
	{ 5, 13, 15, 16, 24, 26, 30, 31, 32, 33, 35, 36, 39, 41, 42 },                                            //a5
	{ 6, 14, 16, 17, 25, 27, 31, 32, 33, 34, 36, 37, 40, 42, 43 },                                            //a6
	{ 7, 12, 16, 21, 22, 24, 26, 29, 32, 35, 36, 39, 40, 43 },                                                //a7
	{ 8, 12, 13, 15, 16, 18, 21, 23, 24, 25, 27, 28, 29, 30, 34, 38, 39 },                                    //a8
	{ 9, 12, 13, 14, 15, 18, 19, 21, 25, 26, 30, 31, 33, 34, 35, 36, 37, 38, 41 },                            //a9
	{ 10, 13, 14, 15, 16, 19, 20, 22, 26, 27, 31, 32, 34, 35, 36, 37, 38, 39, 42 },                           //a10
	{ 11, 14, 15, 16, 17, 20, 21, 23, 27, 28, 32, 33, 35, 36, 37, 38, 39, 40, 43 }                            //a11
};

const vector<vector<int> > Generate_Matrix = {
	{ 0, 4, 5, 8, 9 },					//d0参与这么多校验
	{ 1, 5, 6, 9, 10 },					//d1参与这么多的校验
	{ 2, 6, 7, 10, 11 },
	{ 0, 4, 5, 7, 10, 11 },
	{ 0, 1, 3, 4, 6, 10, 11 },
	{ 0, 1, 2, 3, 7, 8, 10, 11 },
	{ 0, 1, 2, 5, 9, 10, 11 },
	{ 0, 1, 2, 4, 5, 6, 8, 11 },
	{ 0, 1, 2, 4, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 5, 7, 8, 9, 10, 11 },
	{ 0, 2, 5, 6, 9, 11 },
	{ 0, 1, 4, 5, 6, 7, 8 },
	{ 1, 2, 5, 6, 7, 8, 9 },
	{ 2, 3, 6, 7, 8, 9, 10 },
	{ 3, 4, 7, 8, 9, 10, 11 },
	{ 0, 3, 9, 11 },
	{ 0, 1, 3, 5, 8 },
	{ 1, 2, 4, 6, 9 },
	{ 2, 3, 5, 7, 10 },
	{ 3, 4, 6, 8, 11 },
	{ 0, 3, 7, 8, 9, 10 },
	{ 1, 4, 8, 9, 10, 11 },
	{ 0, 2, 3, 4, 8, 9, 11 },
	{ 0, 1, 8, 9 },
	{ 1, 2, 9, 10 },
	{ 2, 3, 10, 11 },
	{ 0, 5, 8, 10, 11 },
	{ 0, 1, 3, 4, 5, 6, 8, 9, 10, 11 },
	{ 0, 1, 2, 3, 6, 7, 8, 9, 11 },
	{ 0, 1, 2, 5, 7, 9 },
	{ 1, 2, 3, 6, 8, 10 },
	{ 2, 3, 4, 7, 9, 11 }
};

const vector<vector<int> > Adjoint_Matrix = {
	{ 0, 4, 5, 8, 9 },		//d0参与的校验
	{ 1, 5, 6, 9, 10 },
	{ 2, 6, 7, 10, 11 },
	{ 0, 4, 5, 7, 10, 11 },
	{ 0, 1, 3, 4, 6, 10, 11 },
	{ 0, 1, 2, 3, 7, 8, 10, 11 },
	{ 0, 1, 2, 5, 9, 10, 11 },
	{ 0, 1, 2, 4, 5, 6, 8, 11 },
	{ 0, 1, 2, 4, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 5, 7, 8, 9, 10, 11 },
	{ 0, 2, 5, 6, 9, 11 },
	{ 0, 1, 4, 5, 6, 7, 8 },
	{ 1, 2, 5, 6, 7, 8, 9 },
	{ 2, 3, 6, 7, 8, 9, 10 },
	{ 3, 4, 7, 8, 9, 10, 11 },
	{ 0, 3, 9, 11 },
	{ 0, 1, 3, 5, 8 },
	{ 1, 2, 4, 6, 9 },
	{ 2, 3, 5, 7, 10 },
	{ 3, 4, 6, 8, 11 },
	{ 0, 3, 7, 8, 9, 10 },
	{ 1, 4, 8, 9, 10, 11 },
	{ 0, 2, 3, 4, 8, 9, 11 },
	{ 0, 1, 8, 9 },
	{ 1, 2, 9, 10 },
	{ 2, 3, 10, 11 },
	{ 0, 5, 8, 10, 11 },
	{ 0, 1, 3, 4, 5, 6, 8, 9, 10, 11 },
	{ 0, 1, 2, 3, 6, 7, 8, 9, 11 },
	{ 0, 1, 2, 5, 7, 9 },
	{ 1, 2, 3, 6, 8, 10 },
	{ 2, 3, 4, 7, 9, 11 },
	{ 0 },
	{ 1 },
	{ 2 },
	{ 3 },
	{ 4 },
	{ 5 },
	{ 6 },
	{ 7 },
	{ 8 },
	{ 9 },
	{ 10 },
	{ 11 }
};

bool BCHCode::getTable()
{
	for (int i = 0; i < 4096; i++) {
		Array_Table[i] = (255 & 0xff) << 8;
		Array_Table[i] |= (255 & 0xff);
	}

	//生成文件流
	string fileName = "errTableBCH_44_32_high_low.txt";
	ifstream fin(fileName, ios::binary);
	if (fin.is_open()) {
		string tmpStr;
		int key, high, low;
		fin >> tmpStr >> tmpStr >> tmpStr;            //去表头
		while (!fin.eof()) {
			fin >> key >> high >> low;
			Array_Table[key] = (high & 0xff) << 8;
			Array_Table[key] |= (low & 0xff);
		}
		//std::cout << "Read errTableBCH_44_32.txt success!" << std::endl;
		return true;
	}
	else {
		std::cout << "Open file errTableBCH_44_32.txt failed!" << std::endl;
		return false;
	}
}

void BCHCode::getParity(bitset<44>& input, bitset<12>& output)
{
	for (int i = 0; i < adjointMatrix.size(); i++) {
		output[i] = 0;
		for (int j = 0; j < adjointMatrix[i].size(); j++) {
			if (input.test(adjointMatrix[i][j])) {
				output.flip(i);
			}
		}
	}
}
int BCHCode::BCHDecode(bitset<44> & input, bitset<44> & output)
{
	bitset<12> parityXOR(0);
	getParity(input, parityXOR);
	//如果没有错误
	if (!parityXOR.any()) {
		output = input;
		//cout << "the code has no errors , return " << endl;
		return 0;
	}
	//如果errTable中有该错误模式，则修复
	int high = (Array_Table[(int)(parityXOR.to_ulong())] >> 8) & 0xff;
	int low = (Array_Table[(int)(parityXOR.to_ulong())]) & 0xff;
	if (high == 255) {
		//cout << "\tthe code has many errors, repair failed, return " << endl;
		return -1;
	}
	if (low == 255) {
		input.flip(high);
	}
	else {
		input.flip(high);
		input.flip(low);
	}
	output = input;
	return 1;
}

int BCHCode::isParityAllZero(bitset<12>& parityXOR)
{
	//如果没有错误
	if (!parityXOR.any()) {
		return 0;
	}
	int high = (Array_Table[(int)(parityXOR.to_ulong())] >> 8) & 0xff;
	//如果errTable中有该错误模式，则修复
	if (high != 255) {
		return 1;
	}
	else {
		return -1;
	}
}


void BCHCode::Encode()
{
	//printf("encoding begin ...\t\n");
	for (int j = 0; j < Generate_Matrix.size(); j++) {
		for (int k = 0; k < Generate_Matrix[j].size(); k++) {
			xor_basic(buffer + (Generate_Matrix[j][k] + 32) * UnitSize, buffer + j * UnitSize, UnitSize);
		}
	}
	//printf("encoding end ...\t\n");
}

void BCHCode::Decode()
{
	char *res = new char[UnitSize * ParityLen];					//一维指针，指向44块信息得到的伴随式的地址;		//存储相关的伴随式
	memset(res, 0, sizeof(char) * UnitSize * ParityLen);
	for (int i = 0; i < Adjoint_Matrix.size(); i++) {
		for (int j = 0; j < Adjoint_Matrix[i].size(); j++) {
			xor_basic(res + Adjoint_Matrix[i][j] * UnitSize, buffer + i * UnitSize, UnitSize);	//这样写对不对？
		}
	}
	long long *lres = (long long *)res;
	long long *llbuffer = (long long *)buffer;

	bitset<12> testParity(0);							//伴随式不全为0， 需要一bit一个bit的去生成12位伴随式，然后定位到哪里出错
	bitset<44> testInput(0);
	bitset<44> testOutput(0);

	for (int i = 0; i < UnitSize / 8; i++) {						//每一块的偏移
		for (int j = 0; j < ParityLen; j++) {
			if (lres[i + j * (UnitSize / 8)]) {
				for (int k = 63; k >= 0; k--) {
					int flag = 0;
					if (IsOne(lres[i + j * (UnitSize / 8)], k)) {
						//std::cout << "pos:\t" << hex << (unsigned int)& lres[i + j * (UnitSize / 8)] << "\t" << k << endl;
						testParity.reset();
						for (int m = 0; m < ParityLen; m++) {
							if (IsOne(lres[i + m * (UnitSize / 8)], k)) {
								testParity[ParityLen - m - 1] = !testParity[ParityLen - m - 1];
							}
						}
						int result = isParityAllZero(testParity);				//判断上步的得到的伴随式是否为0，查看其返回值别忘了赋值给0
						for (int m = 0; m < ParityLen; m++) {
							clrbit(lres[i + m * (UnitSize / 8)], k);
						}
						if (result == 0) {
							cout << "result == 0" << endl;
						}
						else if (result == 1) {

							testInput.reset();
							testOutput.reset();
							for (int m = 0; m < CodeLen; m++) {
								if (IsOne(llbuffer[i + m * (UnitSize / 8)], k)) {
									testInput[CodeLen - m - 1] = !testInput[CodeLen - m - 1];
								}
							}
							//std::cout <<"pos:\t" <<hex << (unsigned int)& lres[i + j * (UnitSize / 8)] << endl;

							int high = (Array_Table[(int)(testParity.to_ulong())] >> 8) & 0xff;
							int low = (Array_Table[(int)(testParity.to_ulong())]) & 0xff;
							if (high != 255) {
								testOutput = testInput.flip(high);
							}
							if (low != 255) {
								testOutput = testInput.flip(low);
							}
							flag = 0;													//无错
							cout << "Using the inner decode success" << endl;
							for (int m = 0; m < CodeLen; m++) {							//写入正确的信息
								if (testOutput[CodeLen - m - 1]) { 						//将解码正确后的信息映射到该块中的对应横行
									setbit(llbuffer[i + m * (UnitSize / 8)], k);
								}
								else {
									clrbit(llbuffer[i + m * (UnitSize / 8)], k);
								}
							}
						}
						else if (result == -1) {
							cout << "the code have many errors, the single code can't  repair,please choose other codes" << endl;
						}
					}

				}
			}
		}
	}
}



bool BCHCode::Check_Is_Right(char * before, char * after, int size)
{
	for (int i = 0; i < size; i++) {
		if (before[i] != after[i]) {				//如果出现前后数据不相等，证明修复失败，返回即可
			cout << "it has error,repair failed" << endl;
			return false;
		}
	}
	return true;
}

void BCHCode::testCode()
{
	freopen("data.out", "w", stdout);								//将输出信息写入文件里

	getTable();														//读表

	struct timeval st, en;											//计时
	long timediff_write = 0;										//编码所用时间
	long timediff_read = 0;											//解码所用时间

	for (int i = 0; i < 1; i++) {

		gettimeofday(&st, NULL);
		Encode();													//编码
		gettimeofday(&en, NULL);
		timediff_write += timeDiffMacroSeconds(st, en);				//完成编码操作


		char * before = new char[CodeLen * UnitSize];
		memset(before, 0, sizeof(char) * UnitSize * CodeLen);
		for (int i = 0; i < CodeLen * UnitSize; i++) {				//将未注入错误之前，记录一下
			before[i] = buffer[0][i];
		}

		reversebit(buffer[0][0], 0); 								//注入错误，第一块的高位进行位翻转
		reversebit(buffer[0][UnitSize * 1], 0); 					//注入错误，第二块的高位进行位翻转
		reversebit(buffer[0][UnitSize * 2], 0); 					//注入错误，第三块的高位进行位翻转

		//reversebit(buffer[1][0], 0); 								//注入错误，第一块的高位进行位翻转
		//reversebit(buffer[1][UnitSize * 1], 0); 					//注入错误，第二块的高位进行位翻转
		//reversebit(buffer[1][UnitSize * 2], 0); 					//注入错误，第三块的高位进行位翻转
		//
		//reversebit(buffer[0][0], 7); 								//注入错误，第一块的高位进行位翻转
		//reversebit(buffer[0][UnitSize * 1], 7); 					//注入错误，第二块的高位进行位翻转
		//reversebit(buffer[0][UnitSize * 2], 7); 					//注入错误，第三块的高位进行位翻转
		//
		//reversebit(buffer[1][0], 7); 								//注入错误，第三块的高位进行位翻转
		//reversebit(buffer[1][UnitSize * 1], 7); 					//注入错误，第二块的高位进行位翻转
		//reversebit(buffer[1][UnitSize * 2], 7); 					//注入错误，第三块的高位进行位翻转

		gettimeofday(&st, NULL);
		Decode();
		gettimeofday(&en, NULL);
		timediff_read += timeDiffMacroSeconds(st, en);

		if (!Check_Is_Right(before, buffer[0], CodeLen * UnitSize)) {
			cout << "Check_Is_Right is false, repair failed" << endl;//判断恢复是否正确
		}
	}


	printf("write time cost %s, %ld us\n", timeFormatMacroToString(timediff_write), timediff_write);
	printf("read time cost %s, %ld us\n", timeFormatMacroToString(timediff_read), timediff_read);

}


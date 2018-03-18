#ifndef BCHCode_h
#define BCHCode_h


#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <bitset>
#include <unordered_map>
#include <time.h>

using namespace std;

const int dataLen = 32;
const int parityLen = 12;
const int codeLen = 44;
const int blocksize = 44;
const int stripesize = 5; //每多少个条纹为1组 目前采用XOR（6,5）5个条纹为一组

class BCHCode {
public:
	void BCHEncode(bitset<dataLen> & input, bitset<codeLen> & output);
	void BCHDecode(bitset<codeLen> & input, bitset<codeLen> & output);
	void getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output);
	void setTable();
	void getTable();
	void testBCH();
	void ErrorCorrection();
	void errTest();
	void testRead(); //测试读的过程
	void testWrite(); //测试写的过程
	void testOneError(); //枚举生成1位错误
	void testTwoErrors(); //枚举生成2位错误
	void testThreeErrors(); //枚举生成3位错误
	void testFunctional(); //功能性测试 包含 枚举生成1位错误 枚举生成2位错误 枚举生成3位错误
	void testUpdateWrite();//测试读修改写
};

#endif /* BCHCode_h */



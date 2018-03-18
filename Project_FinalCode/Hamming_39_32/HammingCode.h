#ifndef HammingCode_h
#define HammingCode_h


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
const int parityLen = 7;
const int codeLen = 39;
const int stripesize = 4; //实行4个编码条纹一组来进行整体的异或编码

class HammingCode {
public:
	void HammingEncode(bitset<dataLen> & input, bitset<codeLen> & output);
	void HammingDecode(bitset<codeLen> & input, bitset<codeLen> & output);
	void getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output);
	void setTable();
	void getTable();
	void testHamming();
	void ErrorCorrection();
	void errTest();
	void testRead(); //测试读的过程
	void testWrite(); //测试写的过程
	void testOneError(); //枚举生成1位错误
	void testTwoErrors(); //枚举生成2位错误
	void testThreeErrors(); //枚举生成3位错误
	void testFunctional(); //功能性测试 包含 枚举生成1位错误 枚举生成2位错误 枚举生成3位错误
};

#endif /* HammingCode_h */



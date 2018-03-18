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
const int stripesize = 4; //ʵ��4����������һ�������������������

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
	void testRead(); //���Զ��Ĺ���
	void testWrite(); //����д�Ĺ���
	void testOneError(); //ö������1λ����
	void testTwoErrors(); //ö������2λ����
	void testThreeErrors(); //ö������3λ����
	void testFunctional(); //�����Բ��� ���� ö������1λ���� ö������2λ���� ö������3λ����
};

#endif /* HammingCode_h */



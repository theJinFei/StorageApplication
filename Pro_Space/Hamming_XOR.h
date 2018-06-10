#ifndef __HAMMING_XOR_H__
#define __HAMMING_XOR_H__

#include <iostream>
#include "BasicClass.h"
#include <bitset>


class Hamming_XOR : public BasicClass {
public:
	Hamming_XOR() { std::cout << "the Hamming_XOR Constructor is called" << std::endl; }
	Hamming_XOR(char ** buffer, int CodeLen, int DataLen, int ParityLen, int UnitSize, int StripeNum, int Group) : BasicClass(buffer, CodeLen, DataLen, ParityLen, UnitSize, StripeNum, Group) {}
	bool getTable();
	void Encode();			//一次写一组数据
	void Decode();			//一次读一块
	//int HammingDecode(std::bitset<39> & input, std::bitset<39> & output);
	int HammingDecode(long long *a);
	int isParityAllZero(std::bitset<7> & parityXOR);
	bool Check_Is_Right(char *before, char *after, int size);
	void testCode();
	~Hamming_XOR() {}
public:
	static short Array_Table[128];
	static char  **before;
	static std::vector<std::vector<int> > Generate_Matrix;
	static std::vector<std::vector<int> > Adjoint_Matrix;
};
#endif

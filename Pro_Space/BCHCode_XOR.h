#ifndef __BCHCODE_XOR_H__
#define __BCHCODE_XOR_H__

#include <iostream>
#include "BasicClass.h"
#include <bitset>


class BCHCode_XOR: public BasicClass {
public:
	BCHCode_XOR() { std::cout << "the BCHCode_XOR Constructor is called" << std::endl; }
	BCHCode_XOR(char ** buffer, int CodeLen, int DataLen, int ParityLen, int UnitSize, int StripeNum, int Group ) : BasicClass(buffer, CodeLen, DataLen, ParityLen, UnitSize, StripeNum, Group) {} 
	bool getTable();
	void getParity(std::bitset<44>& input, std::bitset<12>& output);
	void Encode();			//一次写一组数据
	void Decode();			//一次读一块
	int BCHDecode(std::bitset<44> & input, std::bitset<44> & output);
	int isParityAllZero(std::bitset<12> & parityXOR);
	bool Check_Is_Right(char *before, char *after, int size);
	void testCode();
	~BCHCode_XOR() {}
public:
	static short Array_Table[4096];
	static char  **before;
	static std::vector<std::vector<int> > adjointMatrix;
	static std::vector<std::vector<int> > Generate_Matrix;
	static std::vector<std::vector<int> > Adjoint_Matrix;
};
#endif

#ifndef __BCHCODE_H__
#define __BCHCODE_H__

#include <iostream>
#include "BasicClass.h"
#include <bitset>

class BCHCode : public BasicClass {
public:
	BCHCode() { std::cout << "the BCHCode Constructor is called" << std::endl; }
	BCHCode(char ** buffer, int CodeLen, int DataLen, int ParityLen, int UnitSize, int StripeNum, int Group) : BasicClass(buffer, CodeLen, DataLen, ParityLen, UnitSize, Group) {}
	bool getTable();
	void getParity(std::bitset<44>& input, std::bitset<12>& output);
	void Encode();			//一次写一组数据
	void Decode();			//一次读一块
	int BCHDecode(std::bitset<44> & input, std::bitset<44> & output);
	int isParityAllZero(std::bitset<12> & parityXOR);
	bool Check_Is_Right(char *before, char *after, int size);
	void testCode();
	~BCHCode() {}
};

#endif

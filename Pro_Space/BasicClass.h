#ifndef __BASICCLASS_H__
#define __BASICCLASS_H__
#include <iostream>
#include <vector>

#define  setbit(x, y) x |= (1L << y) 					//将某一位置1
#define  reversebit(x, y) x ^= (1LL << y) 				//将某一位翻转
#define  clrbit(x,y)  x &= ~(1L << y)					//将某一位置0

class BasicClass {
public:
	int CodeLen;
	int DataLen;
	int ParityLen;
	int UnitSize;
	int Group;
	char ** buffer;
public:
	BasicClass(char ** buffer, int CodeLen, int DataLen, int ParityLen, int UnitSize, int Group) :buffer(buffer), CodeLen(CodeLen), DataLen(DataLen), ParityLen(ParityLen),UnitSize(UnitSize), Group(Group){}
	BasicClass() { std::cout << "the BasicClass Constructor is called" << std::endl; }
	void xor_basic(char* a, char* b, int size);
	bool IsOne(int data, int nPos);
	~BasicClass(){ std::cout << "the BasicClass Destructor is called" << std::endl; }

};
#endif
#ifndef __BASICCLASS_H__
#define __BASICCLASS_H__
#include <iostream>
#include <vector>
#include <x86intrin.h>



#define  setbit(x, y) x |= (1LL << y) 					//将某一位置1
#define  reversebit(x, y) x ^= (1LL << y) 				//将某一位翻转
#define  clrbit(x,y)  x &= ~(1LL << y)					//将某一位置0

class BasicClass {
public:
	int CodeLen;
	int DataLen;
	int ParityLen;
	int UnitSize;
	int Group;
	int StripeNum;
	char ** buffer;

public:
	BasicClass(char ** buffer, int CodeLen, int DataLen, int ParityLen, int UnitSize, int StripeNum, int Group) :buffer(buffer), CodeLen(CodeLen), DataLen(DataLen), ParityLen(ParityLen),UnitSize(UnitSize), StripeNum(StripeNum), Group(Group){}
	BasicClass() { std::cout << "the BasicClass Constructor is called" << std::endl; }
	void xor_basic(char* a, char* b, int size);
	void xor_sse_4loop(char* a, char* b, int size);
	void xor_avx_4loop(char* a, char* b, int size);
	bool IsOne(long long data, int nPos);
	~BasicClass(){ std::cout << "the BasicClass Destructor is called" << std::endl; }

};

inline void BasicClass::xor_avx_4loop(char* a, char* b, int size)
{
	int nb_iters = size / 32 / 4;
	__m256* l = (__m256*)a;
	__m256* r = (__m256*)b;
	for (int i = 0; i < nb_iters; ++i, l += 4, r += 4)
	{
		_mm256_store_ps((float*)l, _mm256_xor_ps(*l, *r));
		_mm256_store_ps((float*)(l + 1), _mm256_xor_ps(*(l + 1), *(r + 1)));
		_mm256_store_ps((float*)(l + 2), _mm256_xor_ps(*(l + 2), *(r + 2)));
		_mm256_store_ps((float*)(l + 3), _mm256_xor_ps(*(l + 3), *(r + 3)));
	}
}
#endif
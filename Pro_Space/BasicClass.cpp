#include "BasicClass.h"
#include <iostream>

void BasicClass::xor_basic(char* a, char* b, int size)				//基础的相关异或方法，为64位整形（long long结构）同时进行异或操作
{
	//printf("xor_basic begin ...\t\n");
	long long * la = (long long *)a;
	long long * lb = (long long *)b;
	for (int i = 0; i < size / 8; i++)								//需要注意size/8操作，原来的是char的size，现在改为了long long
		la[i] ^= lb[i];
	//for (int i = size / 8 * 8; i < size; i++)						//防止出现除不尽的情况，剩下的用普通的char与char异或即可
	//	a[i] ^= b[i];
	//printf("xor_basic end ...\t\n");
}

void BasicClass::xor_sse_4loop(char* a, char* b, int size)				//基础的相关异或方法，为64位整形（long long结构）同时进行异或操作
{
	int nb_iters = size / 16 / 4;
	__m128i* l = (__m128i*)a;
	__m128i* r = (__m128i*)b;
	for (int i = 0; i < nb_iters; ++i, l += 4, r += 4)
	{
		_mm_store_si128(l, _mm_xor_si128(*l, *r));
		_mm_store_si128(l + 1, _mm_xor_si128(*(l + 1), *(r + 1)));
		_mm_store_si128(l + 2, _mm_xor_si128(*(l + 2), *(r + 2)));
		_mm_store_si128(l + 3, _mm_xor_si128(*(l + 3), *(r + 3)));
	}
}



bool BasicClass::IsOne(long long data, int nPos)
{
	if (nPos > sizeof(data) * 8)
		return false;
	return data & (1LL << nPos);
}

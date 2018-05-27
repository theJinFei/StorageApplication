#include "BasicClass.h"
#include <iostream>

void BasicClass::xor_basic(char* a, char* b, int size)				//基础的相关异或方法，为64位整形（long long结构）同时进行异或操作
{
	//printf("xor_basic begin ...\t\n");
	long long * la = (long long *)a;
	long long * lb = (long long *)b;
	for (int i = 0; i < size / 8; i++)								//需要注意size/8操作，原来的是char的size，现在改为了long long
		la[i] ^= lb[i];
	for (int i = size / 8 * 8; i < size; i++)						//防止出现除不尽的情况，剩下的用普通的char与char异或即可
		a[i] ^= b[i];
	//printf("xor_basic end ...\t\n");
}

bool BasicClass::IsOne(int data, int nPos)
{
	if (nPos > sizeof(data) * 8)
		return false;
	return data & (1LL << nPos);
}

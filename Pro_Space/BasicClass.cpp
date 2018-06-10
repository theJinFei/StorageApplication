#include "BasicClass.h"
#include <iostream>

void BasicClass::xor_basic(char* a, char* b, int size)				//�����������򷽷���Ϊ64λ���Σ�long long�ṹ��ͬʱ����������
{
	//printf("xor_basic begin ...\t\n");
	long long * la = (long long *)a;
	long long * lb = (long long *)b;
	for (int i = 0; i < size / 8; i++)								//��Ҫע��size/8������ԭ������char��size�����ڸ�Ϊ��long long
		la[i] ^= lb[i];
	//for (int i = size / 8 * 8; i < size; i++)						//��ֹ���ֳ������������ʣ�µ�����ͨ��char��char��򼴿�
	//	a[i] ^= b[i];
	//printf("xor_basic end ...\t\n");
}

void BasicClass::xor_sse_4loop(char* a, char* b, int size)				//�����������򷽷���Ϊ64λ���Σ�long long�ṹ��ͬʱ����������
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

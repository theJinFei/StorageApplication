#include "BasicClass.h"
#include <iostream>

void BasicClass::xor_basic(char* a, char* b, int size)				//�����������򷽷���Ϊ64λ���Σ�long long�ṹ��ͬʱ����������
{
	//printf("xor_basic begin ...\t\n");
	long long * la = (long long *)a;
	long long * lb = (long long *)b;
	for (int i = 0; i < size / 8; i++)								//��Ҫע��size/8������ԭ������char��size�����ڸ�Ϊ��long long
		la[i] ^= lb[i];
	for (int i = size / 8 * 8; i < size; i++)						//��ֹ���ֳ������������ʣ�µ�����ͨ��char��char��򼴿�
		a[i] ^= b[i];
	//printf("xor_basic end ...\t\n");
}

bool BasicClass::IsOne(int data, int nPos)
{
	if (nPos > sizeof(data) * 8)
		return false;
	return data & (1LL << nPos);
}

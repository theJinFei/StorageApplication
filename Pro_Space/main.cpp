#include "BasicClass.h"
#include "main.h"
#include "BCHCode_XOR.h"

#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

#ifdef _WIN32
const char * filename = "G:\\VS_C\\Project_FinalCode\\bch_44_32_block\\TextFile1.txt";		//Win32�ļ���ַ
#else
const char * filename = "TextFile1.txt";													//Linux�ļ���ַ
#endif

#ifdef _WIN32											//��������
#include <intrin.h>
#include <windows.h>
#define INTRIN_SSE
#else
#include <x86intrin.h>
#include <sys/time.h>
#define INTRIN_SSE
#define _aligned_malloc(a, b) memalign(b, a)

#if __AVX512F_
#define INTRIN_AVX_512
#endif
#if __AVX__
#define INTRIN_AVX
#endif
#if __SSE__
#define INTRIN_SSE
#endif

#endif// _WIN32

#define  UnitSize 8 													//�ֽ� ÿ��Ԫ�ص��ֽ���
#define  CodeLen 44 													//����ĳ���
#define  ParityLen 12 													//У��λ�ĸ���
#define  DataLen 32 													//���ݵĳ���
#define	 StripeNum 5 													//���Ƶĸ���
#define  Group 1024 * 1024 * 1024 / DataLen / StripeNum / UnitSize		//�ܹ���Ҫ����ʵ�����


#ifdef _WIN32
extern int gettimeofday(struct timeval *tp, void *tzp);
#endif
#ifdef _WIN32
extern int gettimeofday(struct timeval *tp, void *tzp)			//Win32���¶����ʱ�ĺ���������Ϊus
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;

	return (0);
}

extern char* timeFormatMacroToString(long cost) {
	static char buf[256];
	if (cost >= 1000 * 1000 * 60) {
		cost /= 1000;
		sprintf_s(buf, "%d min %.3f s", cost / 1000 / 60, cost % (6000)*1.0 / 1000);
	}
	else if (cost >= 1000 * 1000) {
		cost /= 1000;
		sprintf_s(buf, "%.3f s", cost*1.0 / 1000);
	}
	else if (cost >= 1000) {
		sprintf_s(buf, "%.3f ms", cost*1.0 / 1000);
	}
	else {
		sprintf_s(buf, "%d us", cost);
	}
	return buf;
}
#else
extern char* timeFormatMacroToString(long cost) {				//Linuxƽ̨�µļ�ʱ��������Ҫ�õ�snprintf
	static char buf[256];
	if (cost >= 1000 * 1000 * 60) {
		cost /= 1000;
		snprintf(buf, sizeof(buf), "%d min %.3f s", cost / 1000 / 60, cost % (6000)*1.0 / 1000);
	}
	else if (cost >= 1000 * 1000) {
		cost /= 1000;
		snprintf(buf, sizeof(buf), "%.3f s", cost*1.0 / 1000);
	}
	else if (cost >= 1000) {
		snprintf(buf, sizeof(buf), "%.3f ms", cost*1.0 / 1000);
	}
	else {
		snprintf(buf, sizeof(buf), "%d us", cost);
	}
	return buf;
}
#endif

extern long timeDiffMacroSeconds(timeval& start, timeval& end) {
	return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

void readData(char **buffer)
{
	for (int i = 0; i < StripeNum + 1; i++) {					//���г�ʼ����һЩ����
		buffer[i] = new char[CodeLen * UnitSize];
		memset(buffer[i], 0, sizeof(char) * CodeLen * UnitSize);
	}

	ifstream fin(filename);

	if (fin.is_open) {
		for (int i = 0; i < StripeNum; i++) {
			fin.read(buffer[i], DataLen * UnitSize);				// ���и�ֵ��һ��д1���飨��StripeNum * UnitSize * DataLen��
		}
		cout << "read data successfully !" << endl;
	}
	else {
		cout << "read data failed!" << endl;
	}

}
int main()
{
	char ** buffer = new char*[StripeNum + 1];									//һ����άָ�룬ָ��6*44����Ϣ�ĵ�ַ������5 * 44Ϊ��Ӧ����Ϣ��Ԫ

	readData(buffer);

	BCHCode_XOR *bchCode_XOR = new BCHCode_XOR(buffer, CodeLen, DataLen, ParityLen, UnitSize, StripeNum, Group);

	bchCode_XOR->testCode();

	return 0;

}
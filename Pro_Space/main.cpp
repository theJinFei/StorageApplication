#include "BasicClass.h"
#include "main.h"
#include "BCHCode_XOR.h"

#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

#ifdef _WIN32
const char * filename = "G:\\VS_C\\Project_FinalCode\\bch_44_32_block\\TextFile1.txt";		//Win32文件地址
#else
const char * filename = "TextFile1.txt";													//Linux文件地址
#endif

#ifdef _WIN32											//条件编译
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

#define  UnitSize 8 													//字节 每个元素的字节数
#define  CodeLen 44 													//编码的长度
#define  ParityLen 12 													//校验位的个数
#define  DataLen 32 													//数据的长度
#define	 StripeNum 5 													//条纹的个数
#define  Group 1024 * 1024 * 1024 / DataLen / StripeNum / UnitSize		//总共需要做的实验次数


#ifdef _WIN32
extern int gettimeofday(struct timeval *tp, void *tzp);
#endif
#ifdef _WIN32
extern int gettimeofday(struct timeval *tp, void *tzp)			//Win32重新定义计时的函数，精度为us
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
extern char* timeFormatMacroToString(long cost) {				//Linux平台下的计时函数，需要用到snprintf
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
	for (int i = 0; i < StripeNum + 1; i++) {					//进行初始化的一些操作
		buffer[i] = new char[CodeLen * UnitSize];
		memset(buffer[i], 0, sizeof(char) * CodeLen * UnitSize);
	}

	ifstream fin(filename);

	if (fin.is_open) {
		for (int i = 0; i < StripeNum; i++) {
			fin.read(buffer[i], DataLen * UnitSize);				// 进行赋值，一次写1个块（即StripeNum * UnitSize * DataLen）
		}
		cout << "read data successfully !" << endl;
	}
	else {
		cout << "read data failed!" << endl;
	}

}
int main()
{
	char ** buffer = new char*[StripeNum + 1];									//一个二维指针，指向6*44块信息的地址，其中5 * 44为相应的信息单元

	readData(buffer);

	BCHCode_XOR *bchCode_XOR = new BCHCode_XOR(buffer, CodeLen, DataLen, ParityLen, UnitSize, StripeNum, Group);

	bchCode_XOR->testCode();

	return 0;

}
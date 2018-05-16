#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <bitset>
#include <time.h>
using namespace  std;

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

#ifdef INTRIN_SSE
#define INTRIN_SSE_4LOOP
#endif


#ifdef _WIN32
int gettimeofday(struct timeval *tp, void *tzp);
#endif
#ifdef _WIN32
int gettimeofday(struct timeval *tp, void *tzp)			//Win32���¶����ʱ�ĺ���������Ϊus
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

char* timeFormatMacroToString(long cost) {
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
char* timeFormatMacroToString(long cost) {				//Linuxƽ̨�µļ�ʱ��������Ҫ�õ�snprintf
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

long timeDiffMacroSeconds(timeval& start, timeval& end) {
	return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

#ifdef _WIN32
const char * filename = "G:\\VS_C\\Project_FinalCode\\bch_44_32_block\\TextFile1.txt";		//Win32�ļ���ַ
#else
const char * filename = "TextFile1.txt";													//Linux�ļ���ַ
#endif


																							//BCH(44,32)�����ɾ��� ��12��У�����32-43��
const vector<vector<int> > Generate_Matrix = {
	{ 29, 28, 27, 26, 23, 22, 20, 16, 15, 11, 10, 8, 7, 6, 5, 4, 3, 0 }, 									//32
	{ 30, 29, 28, 27, 24, 23, 21, 17, 16, 12, 11, 9, 8, 7, 6, 5, 4, 1 }, 									//33
	{ 31, 30, 29, 28, 25, 24, 22, 18, 17, 13, 12, 10, 9, 8, 7, 6, 5, 2 }, 									//34
	{ 31, 30, 28, 27, 25, 22, 20, 19, 18, 16, 15, 14, 13, 9, 5, 4 }, 										//35
	{ 31, 27, 22, 21, 19, 17, 14, 11, 8, 7, 4, 3, 0 }, 														//36
	{ 29, 27, 26, 18, 16, 12, 11, 10, 9, 7, 6, 3, 1, 0 }, 													//37
	{ 30, 28, 27, 19, 17, 13, 12, 11, 10, 8, 7, 4, 2, 1 }, 													//38
	{ 31, 29, 28, 20, 18, 14, 13, 12, 11, 9, 8, 5, 3, 2 }, 													//39
	{ 30, 28, 27, 26, 23, 22, 21, 20, 19, 16, 14, 13, 12, 11, 9, 8, 7, 5, 0 }, 								//40
	{ 31, 29, 28, 27, 24, 23, 22, 21, 20, 17, 15, 14, 13, 12, 10, 9, 8, 6, 1, 0 }, 							//41
	{ 30, 27, 26, 25, 24, 21, 20, 18, 14, 13, 9, 8, 6, 5, 4, 3, 2, 1 }, 									//42
	{ 31, 28, 27, 26, 25, 22, 21, 19, 15, 14, 10, 9, 7, 6, 5, 4, 3, 2 } 									//43
};
//BCH(44,32)��У����� ��12��У���ӣ�0-11��
const vector<vector<int> > Adjoint_Matrix = {
	{ 32, 29, 28, 27, 26, 23, 22, 20, 16, 15, 11, 10, 8, 7, 6, 5, 4, 3, 0 }, 								//S0
	{ 33, 30, 29, 28, 27, 24, 23, 21, 17, 16, 12, 11, 9, 8, 7, 6, 5, 4, 1 }, 								//S1
	{ 34, 31, 30, 29, 28, 25, 24, 22, 18, 17, 13, 12, 10, 9, 8, 7, 6, 5, 2 }, 								//S2
	{ 35, 31, 30, 28, 27, 25, 22, 20, 19, 18, 16, 15, 14, 13, 9, 5, 4 }, 									//S3
	{ 36, 31, 27, 22, 21, 19, 17, 14, 11, 8, 7, 4, 3, 0 }, 													//S4
	{ 37, 29, 27, 26, 18, 16, 12, 11, 10, 9, 7, 6, 3, 1, 0 }, 												//S5
	{ 38, 30, 28, 27, 19, 17, 13, 12, 11, 10, 8, 7, 4, 2, 1 }, 												//S6
	{ 39, 31, 29, 28, 20, 18, 14, 13, 12, 11, 9, 8, 5, 3, 2 }, 												//S7
	{ 40, 30, 28, 27, 26, 23, 22, 21, 20, 19, 16, 14, 13, 12, 11, 9, 8, 7, 5, 0 }, 							//S8
	{ 41, 31, 29, 28, 27, 24, 23, 22, 21, 20, 17, 15, 14, 13, 12, 10, 9, 8, 6, 1, 0 }, 						//S9
	{ 42, 30, 27, 26, 25, 24, 21, 20, 18, 14, 13, 9, 8, 6, 5, 4, 3, 2, 1 }, 								//S10
	{ 43, 31, 28, 27, 26, 25, 22, 21, 19, 15, 14, 10, 9, 7, 6, 5, 4, 3, 2 } 								//S11
};

//  BCH(44, 32)У����󣬣�bitsetʹ�ã���ʵ�����¾�У��λ�����ֵ
const vector<vector<int>> adjointMatrix = {
	{ 0, 12, 15, 16, 17, 18, 21, 22, 24, 28, 29, 33, 34, 36, 37, 38, 39, 40, 41 },                            //a0
	{ 1, 13, 16, 17, 18, 19, 22, 23, 25, 29, 30, 34, 35, 37, 38, 39, 40, 41, 42 },                            //a1
	{ 2, 12, 14, 15, 16, 19, 20, 21, 22, 23, 26, 28, 29, 30, 31, 33, 34, 35, 37, 42, 43 },                    //a2
	{ 3, 13, 15, 16, 17, 20, 21, 22, 23, 24, 27, 29, 30, 31, 32, 34, 35, 36, 38, 43 },                        //a3
	{ 4, 12, 14, 15, 23, 25, 29, 30, 31, 32, 34, 35, 38, 40, 41 },                                            //a4
	{ 5, 13, 15, 16, 24, 26, 30, 31, 32, 33, 35, 36, 39, 41, 42 },                                            //a5
	{ 6, 14, 16, 17, 25, 27, 31, 32, 33, 34, 36, 37, 40, 42, 43 },                                            //a6
	{ 7, 12, 16, 21, 22, 24, 26, 29, 32, 35, 36, 39, 40, 43 },                                                //a7
	{ 8, 12, 13, 15, 16, 18, 21, 23, 24, 25, 27, 28, 29, 30, 34, 38, 39 },                                    //a8
	{ 9, 12, 13, 14, 15, 18, 19, 21, 25, 26, 30, 31, 33, 34, 35, 36, 37, 38, 41 },                            //a9
	{ 10, 13, 14, 15, 16, 19, 20, 22, 26, 27, 31, 32, 34, 35, 36, 37, 38, 39, 42 },                           //a10
	{ 11, 14, 15, 16, 17, 20, 21, 23, 27, 28, 32, 33, 35, 36, 37, 38, 39, 40, 43 }                            //a11
};




#define  UnitSize 128 									//�ֽ� ÿ��Ԫ�ص��ֽ���
#define  CodeLen 44 									//����ĳ���
#define  ParityLen 12 									//У��λ�ĸ���
#define  DataLen 32 									//���ݵĳ���
#define	 StripeNum 5 									//���Ƶĸ���
#define  Piece CodeLen * UnitSize / 4					//��һ��ֳ�4�ݽ���������ı���


#define  setbit(x, y) x |= (1 << y) 					//��ĳһλ��1
#define  reversebit(x, y) x ^= (1 << y) 				//��ĳһλ��ת
#define  clrbit(x,y)  x&=~(1<<y)						//��ĳһλ��0


//#define MEMORY_ALIGN //memory control

unordered_map<bitset<ParityLen>, bitset<CodeLen>> errTable;		//��������ݽṹ��keyΪ��Ӧ�İ���ʽ��12-bit����valueΪ��Ӧ�Ĵ���ͼ����44-bit��
bitset<ParityLen> parityXOR;									//�洢����ʽ


inline void xor_basic(char* a, char* b, int size)				//�����������򷽷���Ϊ64λ���Σ�long long�ṹ��ͬʱ����������
{
	long long * la = (long long *)a;
	long long * lb = (long long *)b;
	for (int i = 0; i < size / 8; i++)							//��Ҫע��size/8������ԭ������char��size�����ڸ�Ϊ��long long
		la[i] ^= lb[i];
	for (int i = size / 8 * 8; i < size; i++)					//��ֹ���ֳ������������ʣ�µ�����ͨ��char��char��򼴿�
		a[i] ^= b[i];
}

#ifdef INTRIN_SSE
inline void xor_sse(char* a, char* b, int size)					//ʦ��д��SSE��ش���
{

	int nb_iters = size / 16;
	int left_size = size % 16;

	__m128* l = (__m128*)a;
	__m128* r = (__m128*)b;

	for (int i = 0; i < nb_iters; ++i, ++l, ++r)
	{
#ifdef MEMORY_ALIGN
		_mm_store_ps((float*)l, _mm_xor_ps(*l, *r));
#else
		__m128 l0 = _mm_loadu_ps((float*)l);
		__m128 r0 = _mm_loadu_ps((float*)r);
		_mm_storeu_ps((float*)l, _mm_xor_ps(l0, r0));

#endif
	}
	xor_basic(a, b, left_size);

}
void xor_sse_4loop(char* a, char* b, int size)
{
	int nb_iters = size / 16 / 4;
	int left_size = size % (16 * 4);
	__m128i* l = (__m128i*)a;
	__m128i* r = (__m128i*)b;
	for (int i = 0; i < nb_iters; ++i, l += 4, r += 4)
	{
#ifdef MEMORY_ALIGN
		_mm_store_si128(l, _mm_xor_si128(*l, *r));
		_mm_store_si128(l + 1, _mm_xor_si128(*(l + 1), *(r + 1)));
		_mm_store_si128(l + 2, _mm_xor_si128(*(l + 2), *(r + 2)));
		_mm_store_si128(l + 3, _mm_xor_si128(*(l + 3), *(r + 3)));
#else
		__m128 l0 = _mm_loadu_ps((float*)l);
		__m128 r0 = _mm_loadu_ps((float*)r);
		_mm_storeu_ps((float*)l, _mm_xor_ps(l0, r0));
		__m128 l1 = _mm_loadu_ps((float*)(l + 1));
		__m128 r1 = _mm_loadu_ps((float*)(r + 1));
		_mm_storeu_ps((float*)(l + 1), _mm_xor_ps(l1, r1));
		__m128 l2 = _mm_loadu_ps((float*)(l + 2));
		__m128 r2 = _mm_loadu_ps((float*)(r + 2));
		_mm_storeu_ps((float*)(l + 2), _mm_xor_ps(l2, r2));
		__m128 l3 = _mm_loadu_ps((float*)(l + 3));
		__m128 r3 = _mm_loadu_ps((float*)(r + 3));
		_mm_storeu_ps((float*)(l + 3), _mm_xor_ps(l3, r3));
#endif
	}
	xor_basic(a, b, left_size);
}
#endif


/**
���������ű��浽errTable�У�keyΪ��Ӧ�İ���ʽ��12-bit����ֵΪ����ͼ��(44-bit)
*/
void getTable() {
	//�����ļ���
	string fileName = "errTableBCH_44_32.txt";
	ifstream fin(fileName, ios::binary);
	string tmpStr;
	bitset<CodeLen> tmp1;
	bitset<ParityLen> tmp2;
	fin >> tmpStr >> tmpStr;            //ȥ��ͷ
	while (!fin.eof()) {
		fin >> tmp1 >> tmp2;
		errTable[tmp2] = tmp1;			//���뵽�ڴ棬���浽errTable���ݽṹ��
	}
}

/**
��������ʽ

@param input ���յ�������
@param output �������ʽ
*/
void getParityXOR(bitset<CodeLen> & input, bitset<ParityLen> & output) {
	//printf("getParityXOR start...\n");
	for (int i = 0; i < adjointMatrix.size(); i++) {
		output[i] = 0;
		for (int j = 0; j < adjointMatrix[i].size(); j++) {
			//output[i] ^= input[adjointMatrix[i][j]];
			if (input.test(adjointMatrix[i][j])) {
				output.flip(i);
			}
		}
	}
	//printf("getParityXOR end...\n");
}


/**
����

@param input ���뵽������
@param output ���������
��Ϊ���������
���� 0 ���޴���
���� 1 �����ڽ���
���� -1 ���޷����ڽ���
*/
int BCHDecode(bitset<CodeLen> & input, bitset<CodeLen> & output) {
	getParityXOR(input, parityXOR);
	//���û�д���
	if (!parityXOR.any()) {
		output = input;
		//cout << "the code has no errors , return " << endl;
		return 0;
	}
	//���errTable���иô���ģʽ�����޸�
	auto existTable = errTable.find(parityXOR);
	if (existTable != errTable.end()) {
		output = input ^ errTable[parityXOR];
		//cout << "the code repair success, return " << endl;
		return 1;
	}
	else {
		output = input;
		//cout << "\tthe code has many errors, repair failed, return " << endl;
		return -1;
	}
}

/**
�жϰ���ʽ�����

@param input �յ��İ���ʽ
���ط�Ϊ���������
���� 0 ���޴���
���� 1 �����ڽ���
���� -1 ���޷����ڽ���
*/
int isParityAllZero(bitset<ParityLen> & parityXOR) {
	//���û�д���
	if (!parityXOR.any()) {
		return 0;
	}
	//���errTable���иô���ģʽ�����޸�
	auto existTable = errTable.find(parityXOR);
	if (existTable != errTable.end()) {
		return 1;
	}
	else {
		return -1;
	}
}



/**
���� ���У��λ

@param resUnit ��Ҫ��ӵ�У��λ
@param pos ��һ���У��λ����Ӧ��ͬ�ı��������У�
@param data 32λ����
*/
void AppendParity(char * resUnit, const int& pos, char * data) {
	memset(resUnit, 0, sizeof(char) * UnitSize);
	for (int i = 0; i < Generate_Matrix[pos - 32].size(); i++) {
		xor_basic(resUnit, data + Generate_Matrix[pos - 32][i] * UnitSize, UnitSize);
		//xor_sse(resUnit, data + Generate_Matrix[pos - 32][i] * UnitSize, UnitSize);
		//xor_sse_4loop(resUnit, data + Generate_Matrix[pos - 32][i] * UnitSize, UnitSize);
	}
}

/**
���� ��Ӱ���ʽ

@param resUnit ��44����Ϣ�õ��İ���ʽ
@param data �����44������
*/
void AppendSyndrome(char * resUnit, char *data) {
	memset(resUnit, 0, sizeof(char) * UnitSize * ParityLen);
	for (int pos = 0; pos < ParityLen; pos++) {
		for (int i = 0; i < Adjoint_Matrix[pos].size(); i++) {
			xor_basic(resUnit + pos * UnitSize, data + Adjoint_Matrix[pos][i] * UnitSize, UnitSize);
			//xor_sse(resUnit + pos * UnitSize, data + Adjoint_Matrix[pos][i] * UnitSize, UnitSize);
			//xor_sse_4loop(resUnit + pos * UnitSize, data + Adjoint_Matrix[pos][i] * UnitSize, UnitSize);
		}
	}
}

/**
��֤ �Ƿ��ܹ���ȷ����

@param before ע�����ǰ����Ϣ
@param after ע�����󲢽�������Ϣ
@param size 44������ݵĴ�С
*/
bool check_isAllright(char * before, char *after, int size) {
	for (int i = 0; i < size; i++) {
		if (before[i] != after[i]) {				//�������ǰ�����ݲ���ȣ�֤���޸�ʧ�ܣ����ؼ���
			cout << "it has error,return " << endl;
			return false;
		}
	}
	return true;
	//cout << "repair success" << endl;
}


int main()
{

	getTable();													//���Ƚ��ж������

	freopen("data.out", "w", stdout);							//�������Ϣд���ļ���


	const long long Block_Size = StripeNum * DataLen * UnitSize;//����BlockSize

	char ** buffer = NULL;										//һ����άָ�룬ָ��6*44����Ϣ�ĵ�ַ������5 * 44Ϊ��Ӧ����Ϣ��Ԫ
	buffer = new char*[StripeNum + 1];
	for (int i = 0; i < StripeNum + 1; i++) {					//���г�ʼ����һЩ����
		buffer[i] = new char[CodeLen * UnitSize];
		memset(buffer[i], 0, sizeof(char) * CodeLen * UnitSize);
	}

	char *PDHLatinXOR = NULL;
	PDHLatinXOR = new char[Piece * 5];							//����������һ��У��



	ifstream fin(filename);


	for (int i = 0; i < StripeNum; i++) {
		fin.read(buffer[i], DataLen * UnitSize);				// ���и�ֵ��һ��д1���飨��StripeNum * UnitSize * DataLen��
	}

	char *res = new char[UnitSize * ParityLen];					//һάָ�룬ָ��44����Ϣ�õ��İ���ʽ�ĵ�ַ
	memset(res, 0, sizeof(char) * UnitSize * ParityLen);		//��ʼ��

	int flag = 0;												//����Ƿ��ܹ���ȷ�ָ��ı�־λ flagΪ0 ����ȷ�ָ����޴��� ����Ϊ1

	struct timeval st, en;										//��ʱ
	gettimeofday(&st, NULL);
	const int num = 1024 * 1024 * 1024 / Block_Size; 			//1GB / BlockSize,�ó���Ҫ����ô����ʵ��

	long timediff_write = 0;									//��������ʱ��
	long timediff_read = 0;										//��������ʱ��
	for (int m = 0; m < 1; m++) {								//forѭ����������ô����ʵ��

		//���ڱ���
		gettimeofday(&st, NULL);
		for (int i = 0; i < StripeNum; i++) {
			for (int j = 32; j < 44; j++) {
				AppendParity(&buffer[i][j * UnitSize], j, buffer[i]);	//���ڱ��룬��32����Ϣλ ����12У��λ���˺�����Ҫ����StripeNum * 12��������һ���Ǳ�һ�����ݣ�
			}
		}
		//�ּ���� ������ĵ�һ�� ˮƽУ��
		memset(buffer[5], 0, sizeof(char) * CodeLen * UnitSize);
		for (int i = 0; i < StripeNum; i++) {
			xor_basic(buffer[5], buffer[i], CodeLen * UnitSize);
			//xor_sse(buffer[5], buffer[i], CodeLen * UnitSize);
			//xor_sse_4loop(buffer[5], buffer[i], CodeLen * UnitSize);	//�ּ���룬�����ڶ�Ӧλ������򼴿�
		}

		memset(PDHLatinXOR, 0, sizeof(char) * Piece * 5);

		//�ּ���� ������ĵڶ��� ������Ӧ����У��
		for (int i = 0; i < Piece; i++) {
			PDHLatinXOR[i] = buffer[0][i + Piece] ^ buffer[1][i + Piece * 3] ^ buffer[2][i] ^ buffer[3][i + Piece * 2];
			PDHLatinXOR[i + Piece] = buffer[0][i + Piece * 2] ^ buffer[2][i + Piece] ^ buffer[3][i + Piece * 3] ^ buffer[4][i];
			PDHLatinXOR[i + Piece * 2] = buffer[0][i + Piece * 3] ^ buffer[1][i] ^ buffer[2][i + Piece * 2] ^ buffer[4][i + Piece];
			PDHLatinXOR[i + Piece * 3] = buffer[1][i + Piece] ^ buffer[2][i + Piece * 3] ^ buffer[3][i] ^ buffer[4][i + Piece * 2];
			PDHLatinXOR[i + Piece * 4] = buffer[0][i] ^ buffer[1][i + Piece * 2] ^ buffer[3][i + Piece] ^ buffer[4][i + Piece * 3];
		}

		gettimeofday(&en, NULL);
		timediff_write += timeDiffMacroSeconds(st, en);					//��ɱ������


		char  *before = NULL, *after = NULL;							//δע�����֮ǰ�����ע����������޸����������бȽϣ����Ƿ�ָ���ȷ
		before = new char[CodeLen * UnitSize];
		after = new char[CodeLen * UnitSize];

		for (int i = 0; i < CodeLen * UnitSize; i++) {					//��δע�����֮ǰ����¼һ�£�
			before[i] = buffer[0][i];
		}


		reversebit(buffer[0][7 + 0], 7); 								//ע����󣬵�һ��ĸ�λ����λ��ת
		reversebit(buffer[0][7 + UnitSize * 1], 7); 					//ע����󣬵ڶ���ĸ�λ����λ��ת
		reversebit(buffer[0][7 + UnitSize * 2], 7); 					//ע����󣬵�����ĸ�λ����λ��ת

		reversebit(buffer[1][7 + 0], 7); 								//ע����󣬵�һ��ĸ�λ����λ��ת
		reversebit(buffer[1][7 + UnitSize * 1], 7); 					//ע����󣬵ڶ���ĸ�λ����λ��ת
		reversebit(buffer[1][7 + UnitSize * 2], 7); 					//ע����󣬵�����ĸ�λ����λ��ת


		//ע�����󣬽��н������
		gettimeofday(&st, NULL);
		AppendSyndrome(res, buffer[0]);									//��һ�����ݣ���buffer[0]������У�������ˣ����ɰ���ʽ��resΪָ�����ɵ�12λ����ʽ�ĵ�ַ
		for (int j = 0; j < ParityLen * UnitSize; j++) {
			if (res[j]) {												//���������������ɵİ���ʽȫΪ0��֤���޴��󣬷���flagΪ1
				flag = 1;
				break;
			}
		}
		if (!flag) {
			//cout << "the 12-S is zero, it has no errors" << endl;		//���Ϊ0��֤���޴���
		}
		else {
			bitset<ParityLen> testParity(0);							//����ʽ��ȫΪ0�� ��Ҫһbitһ��bit��ȥ����12λ����ʽ��Ȼ��λ���������
			for (int i = 0; i < UnitSize; i++) {
				for (int pos = 7; pos >= 0; pos--) {
					flag = 0;
					testParity.reset();
					for (int j = 0; j < ParityLen; j++) {
						if (res[i + j * UnitSize] & (1 << pos)) { 		//12�� ÿ������ �����ȡֵ�������鿴char(8-bit)��0����1
							testParity[ParityLen - j - 1] = !testParity[ParityLen - j - 1];
						}
					}
					//cout << "testInput_before is " << testInput << endl;
					int res = isParityAllZero(testParity);				//�ж��ϲ��ĵõ��İ���ʽ�Ƿ�Ϊ0���鿴�䷵��ֵ
					if (res == 0) {										//����������Ϊ0��֤����һ�����޴��󣬽���ȡchar�е���һ��bit
						continue;
					}
					else if (res == 1) {								//������Ϊ1��֤���ĵõ��İ���ʽ�ڴ����б����ܹ��ҵ���û�г������ھ���ķ�Χ�����������ڽ��뼴��

						//�������ڽ���
						bitset<CodeLen> testInput(0);
						bitset<CodeLen> testOutput(0);
						for (int j = 0; j < CodeLen; j++) {
							if (buffer[0][i + j * UnitSize] & (1 << pos)) { 		//ȡ�ÿ��еĶ�Ӧ���У�ȡ����ÿһλ��
								testInput[CodeLen - j - 1] = !testInput[CodeLen - j - 1];
							}
						}
						testOutput = testInput ^ errTable[testParity];				//������ͼ�����ɻָ���ȷ����Ϣ
						flag = 0;													//�޴�
						//cout << "Using the inner decode success" << endl;
						//cout << "testOutput is " << testOutput << endl;
						for (int j = 0; j < CodeLen; j++) {							//д����ȷ����Ϣ
							if (testOutput[CodeLen - j - 1]) { 						//��������ȷ�����Ϣӳ�䵽�ÿ��еĶ�Ӧ����
								setbit(buffer[0][i + j * UnitSize], pos);
							}
							else {
								clrbit(buffer[0][i + j * UnitSize], pos);
							}
						}
					}
					else if (res == -1) {											//�������ֵΪ-1,֤�������˶�λ������Ҫ�����ּ�У��

						//�����ּ�У��
						vector<bitset<CodeLen> > testGroupInput(StripeNum + 1, bitset<CodeLen>(0));
						vector<bitset<CodeLen> > testGroupOutput(StripeNum + 1, bitset<CodeLen>(0));
						for (int j = 1; j < StripeNum + 1; j++) {
							for (int k = 0; k < CodeLen; k++) {
								if (buffer[j][i + k * UnitSize] & (1 << pos)) {
									testGroupInput[j][CodeLen - 1 - k] = !testGroupInput[j][CodeLen - 1 - k];	//ȡͬ���������Ӧλ�õĺ���
								}
							}
						}
						for (int j = 1; j < StripeNum + 1; j++) {
							int res2 = BCHDecode(testGroupInput[j], testGroupOutput[j]); 						//ȡ���󣬶Ա�����������ƽ��н���
							if (res2 == -1) {																	//���ͬ���е������������г������ھ����޷��޸������������Ϊ�޸�ʧ�ܣ�����
								//cout << "the group has many errors, repair failed" << endl;
								//flag = 1;																		//ʧ�ܺ�flag��1�����ؼ���
								//break;
								switch (j)																		//��������룬����֧��˫�ݴ���ͬ����������Ʒ�����������ʱ�򣬿������ô˷����ﵽ�޸�������Ƶ�Ŀ��
								{
								case 1:																			//���ڲ�ͬ�����в�ͬ�Ľ���������ڼٶ���0�� ��1�������˴���������������ָ�������Ĵ���
									memset(buffer[0], 0, sizeof(char) * CodeLen * UnitSize);					//������Ӧ����0����ֹԭʼֵ������ֵ����Ӱ��
									memset(buffer[1], 0, sizeof(char) * CodeLen * UnitSize);
									for (int i = 0; i < Piece; i++) {
										buffer[0][i + Piece * 2] = buffer[2][i + Piece] ^ buffer[3][i + Piece * 3] ^ buffer[4][i] ^ PDHLatinXOR[i + Piece];
										buffer[1][i + Piece * 2] = buffer[0][i + Piece * 2] ^ buffer[2][i + Piece * 2] ^ buffer[3][i + Piece * 2] ^ buffer[4][i + Piece * 2] ^ buffer[5][i + Piece * 2];
										
										buffer[0][i]			 = buffer[1][i + Piece * 2] ^ buffer[3][i + Piece] ^ buffer[4][i + Piece * 3] ^ PDHLatinXOR[i + Piece * 4];
										buffer[1][i]			 = buffer[0][i] ^ buffer[2][i] ^ buffer[3][i] ^ buffer[4][i] ^ buffer[5][i];
										
										buffer[0][i + Piece * 3] = buffer[1][i] ^ buffer[2][i + Piece * 2] ^ buffer[4][i + Piece] ^ PDHLatinXOR[i + Piece * 2];
										buffer[1][i + Piece * 3] = buffer[0][i + Piece * 3] ^ buffer[2][i + Piece * 3] ^ buffer[3][i + Piece * 3] ^ buffer[4][i + Piece * 3] ^ buffer[5][i + Piece * 3];
										
										buffer[0][i + Piece]	 = buffer[1][i + Piece * 3] ^ buffer[2][i] ^ buffer[3][i + Piece * 2] ^ PDHLatinXOR[i];
										buffer[1][i + Piece]	 = buffer[0][i + Piece] ^ buffer[2][i + Piece] ^ buffer[3][i + Piece] ^ buffer[4][i + Piece] ^ buffer[5][i + Piece];
									}
									break;
								case 2:
									//֮����˵
									//memset(buffer[0], 0, sizeof(char) * CodeLen * UnitSize);
									//memset(buffer[2], 0, sizeof(char) * CodeLen * UnitSize);
									//for (int i = 0; i < Piece; i++) {
									//	buffer[0][i] = buffer[1][i + Piece * 2] ^ buffer[3][i + Piece] ^ buffer[4][i + Piece * 3] ^ PDHLatinXOR[i + Piece * 4];
									//	buffer[2][i] = buffer[0][i] ^ buffer[1][i] ^ buffer[3][i] ^ buffer[4][i] ^ buffer[5][i];

									//	buffer[0][i] = buffer[1][i + Piece * 2] ^ buffer[3][i + Piece] ^ buffer[4][i + Piece * 3] ^ PDHLatinXOR[i + Piece * 4];
									//	buffer[2][i] = buffer[0][i] ^ buffer[2][i] ^ buffer[3][i] ^ buffer[4][i] ^ buffer[5][i];

									//	buffer[0][i + Piece * 3] = buffer[1][i] ^ buffer[2][i + Piece * 2] ^ buffer[4][i + Piece] ^ PDHLatinXOR[i + Piece * 2];
									//	buffer[2][i + Piece * 3] = buffer[0][i + Piece * 3] ^ buffer[2][i + Piece * 3] ^ buffer[3][i + Piece * 3] ^ buffer[4][i + Piece * 3] ^ buffer[5][i + Piece * 3];

									//	buffer[0][i + Piece] = buffer[1][i + Piece * 3] ^ buffer[2][i] ^ buffer[3][i + Piece * 2] ^ PDHLatinXOR[i];
									//	buffer[2][i + Piece] = buffer[0][i + Piece] ^ buffer[2][i + Piece] ^ buffer[3][i + Piece] ^ buffer[4][i + Piece] ^ buffer[5][i + Piece];
									//}
									//break;
								default:
									flag = 1;
									break;
								}
							}
							else{
								testGroupOutput[0] ^= testGroupOutput[j];										//���ͬ�����Ϣ�ܹ���ȷ���룬�������Ӧ��Ϣ����
							}
						}
						if (!flag) {																			//���һ������޸���ɵı�־λ���Ƿ��޸��ɹ���
							//cout << "Using the Outer XOR decode success" << endl;
							//cout << "testGroupOutput is " << testGroupOutput[0] << endl;
							for (int j = 0; j < CodeLen; j++) {													//����޸��ɹ���д����ȷ����Ϣ
								if (testGroupOutput[0][CodeLen - j - 1]) { 										//��������ȷ�����Ϣӳ�䵽�ÿ��еĶ�Ӧ����
									setbit(buffer[0][i + j * UnitSize], pos);
								}
								else {
									clrbit(buffer[0][i + j * UnitSize], pos);
								}
							}

						}
						else {
							cout << "the group has many errors, repair failed" << endl;
						}

					}
				}
			}

		}

		gettimeofday(&en, NULL);
		timediff_read += timeDiffMacroSeconds(st, en);


		for (int i = 0; i < CodeLen * UnitSize; i++) {			//��ֵ��������Ϣ
			after[i] = buffer[0][i];
		}
		if (!check_isAllright(before, after, CodeLen * UnitSize)) {
			cout << "something happened, repair failed" << endl;//�жϻָ��Ƿ���ȷ
		}	

		if (before) delete[] before;							//�ͷ���Ӧ�ڴ�
		if (after) delete[] after;
	}



	printf("write time cost %s, %ld us\n", timeFormatMacroToString(timediff_write), timediff_write);
	printf("read time cost %s, %ld us\n", timeFormatMacroToString(timediff_read), timediff_read);


	for (int i = 0; i <= StripeNum; i++) {						//�ͷ���Ӧ�ڴ�
		if (buffer[i]) delete[] buffer[i];
	}
	if (buffer) delete[] buffer;
	if (res) delete[] res;

	return 0;
}

#include "Hamming_XOR.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <bitset>
#include <time.h>
#include "main.h"

using namespace  std;
short Hamming_XOR::Array_Table[128] = { 0 };								//��������ݽṹ���±�Ϊ��Ӧ�İ���ʽ��7-bit����ֵΪ��Ӧ�Ĵ���λ��
char**  Hamming_XOR::before = NULL;											//δע�����֮ǰ�����ע����������޸����������бȽϣ����Ƿ�ָ���ȷ


vector<vector<int> > Hamming_XOR::Generate_Matrix = {
	{0, 1, 2, 3, 6},
	{1, 3, 6},
	{1, 4, 6},
	{2, 4, 6},
	{0, 1, 2, 4, 6},
	{3, 4, 6},
	{0, 1, 3, 4, 6},
	{0, 2, 3, 4, 6},
	{0, 1, 5},
	{0, 2, 5},
	{1, 2, 5},
	{0, 3, 5},
	{1, 3, 5},
	{2, 3, 5},
	{0, 4, 5},
	{2, 4, 5},
	{1, 2, 3},
	{0, 1, 3},
	{0, 1, 4},
	{0, 2, 4},
	{1, 2, 4},
	{0, 3, 4},
	{1, 3, 4},
	{2, 3, 4},
	{1, 5, 6},
	{2, 5, 6},
	{0, 1, 2, 5, 6},
	{3, 5, 6},
	{0, 1, 3, 5, 6},
	{0, 2, 3, 5, 6},
	{4, 5, 6},
	{0, 2, 4, 5, 6}
};

vector<vector<int> > Hamming_XOR::Adjoint_Matrix = {
	{0, 1, 2, 3, 6},
	{1, 3, 6},
	{1, 4, 6},
	{2, 4, 6},
	{0, 1, 2, 4, 6},
	{3, 4, 6},
	{0, 1, 3, 4, 6},
	{0, 2, 3, 4, 6},
	{0, 1, 5},
	{0, 2, 5},
	{1, 2, 5},
	{0, 3, 5},
	{1, 3, 5},
	{2, 3, 5},
	{0, 4, 5},
	{2, 4, 5},
	{1, 2, 3},
	{0, 1, 3},
	{0, 1, 4},
	{0, 2, 4},
	{1, 2, 4},
	{0, 3, 4},
	{1, 3, 4},
	{2, 3, 4},
	{1, 5, 6},
	{2, 5, 6},
	{0, 1, 2, 5, 6},
	{3, 5, 6},
	{0, 1, 3, 5, 6},
	{0, 2, 3, 5, 6},
	{4, 5, 6},
	{0, 2, 4, 5, 6},
	{0},
	{1},
	{2},
	{3},
	{4},
	{5},
	{6}
};



bool Hamming_XOR::getTable()
{
	for (int i = 0; i < 128; i++) {
		Array_Table[i] = (255 & 0xff) << 8;
		Array_Table[i] |= (255 & 0xff);
	}

	//�����ļ���
	string fileName = "errTableHamming_39_32_high_low.txt";
	ifstream fin(fileName, ios::binary);
	if (fin.is_open()) {
		string tmpStr;
		int key, high, low;
		fin >> tmpStr >> tmpStr >> tmpStr;            //ȥ��ͷ
		while (!fin.eof()) {
			fin >> key >> high >> low;
			Array_Table[key] = (high & 0xff) << 8;
			Array_Table[key] |= (low & 0xff);
		}
		//std::cout << "Read errTableHamming_39_32_high_low.txt success!" << std::endl;
		return true;
	}
	else {
		std::cout << "Open file errTableHamming_39_32_high_low.txt failed!" << std::endl;
		return false;
	}
}


bool ISOne(char data, int nPos) {
	return data & (1 << nPos);
}

/*int Hamming_XOR::HammingDecode(bitset<39> & input, bitset<39> & output)
{
	bitset<7> parityXOR(0);
	//getParity(input, parityXOR);
	for (int i = 0; i < Adjoint_Matrix.size(); i++) {
		for (int j = 0; j < Adjoint_Matrix[i].size(); j++) {
			//xor_basic(output[Adjoint_Matrix[i][j]], buffer[num] + i * UnitSize, UnitSize);	//����д�Բ��ԣ�
			parityXOR[Adjoint_Matrix[i][j]] = parityXOR[Adjoint_Matrix[i][j]] ^ input[38 - i];
		}
	}
	//char *a = new char[7];
	//memset(a, 0, sizeof(char) * 7);
	//for (int i = 0; i < Adjoint_Matrix.size(); i++) {
	//	for (int j = 0; j < Adjoint_Matrix[i].size(); j++) {
	//		//xor_basic(output[Adjoint_Matrix[i][j]], buffer[num] + i * UnitSize, UnitSize);	//����д�Բ��ԣ�
	//		a[Adjoint_Matrix[i][j]] = a[Adjoint_Matrix[i][j]] ^ input[38 - i];
	//	}
	//}
	//bitset<7> parityXOR(0);
	//for (int i = 0; i < 7; i++) {
	//	if (ISOne(a[i], 0)) {
	//		parityXOR[6 - i] = 1;
	//	}
	//}
	//int flag = 0;
	//for (int i = 0; i < 7; i++) {
	//	if (a[i] != 0) {
	//		flag = 1;
	//		break;
	//	}
	//}
	//if (!flag) {
	//	output = input;
	//}

	//cout << "HammingDecode parityXOR " << parityXOR << endl;
	//���û�д���
	if (!parityXOR.any()) {
		output = input;
		//cout << "the code has no errors , return " << endl;
		return 0;
	}
	//���errTable���иô���ģʽ�����޸�
	int high = (Array_Table[(int)(parityXOR.to_ulong())] >> 8) & 0xff;
	if (high == 255) {
		cout << "\tthe code has many errors, repair failed, return " << endl;
		return -1;
	}
	else {
		input.flip(high);
	}
	output = input;
	return 1;
}*/

int BintoDec(int *a) {
	//int i = 0, res = 0;
	//while (i <= 6) {
	//	if (a[6 - i]) {
	//		res += pow(2, i);
	//	}
	//	i++;
	//}
	//return res;
	//����ת���ɶ�����
	int res = 0;
	for (int i = 6; i >= 0; i--) {
		res |= a[i] << (6 - i);
	}
	return res;
}

int Hamming_XOR::HammingDecode(long long *a)
{
	int *parity = new int[7];
	memset(parity, 0, sizeof(int) * 7);
	for (int i = 0; i < Adjoint_Matrix.size(); i++) {
		for (int j = 0; j < Adjoint_Matrix[i].size(); j++) {
			//xor_basic(output[Adjoint_Matrix[i][j]], buffer[num] + i * UnitSize, UnitSize);	//����д�Բ��ԣ�
			parity[Adjoint_Matrix[i][j]] = parity[Adjoint_Matrix[i][j]] ^ a[i];
		}
	}
	int flag = 0;
	for (int i = 0; i < 7; i++) {
		if (parity[i]) {
			flag = 1;
		}
	}
	if (!flag) {
		return 0;
	}
	//cout << endl;

	int res = BintoDec(parity);

	int high = (Array_Table[res] >> 8) & 0xff;
	//cout << "high" << high << endl;
	if (high == 255) {
		//cout << "\tthe code has many errors, repair failed, return " << endl;
		return -1;
	}
	else {
		//input.flip(high);
		a[high] = !a[high];
	}
	//output = input;
	return 1;
}

int Hamming_XOR::isParityAllZero(bitset<7>& parityXOR)
{
	//���û�д���
	if (!parityXOR.any()) {
		return 0;
	}
	int high = (Array_Table[(int)(parityXOR.to_ulong())] >> 8) & 0xff;
	//���errTable���иô���ģʽ�����޸�
	if (high != 255) {
		return 1;
	}
	else {
		return -1;
	}
}


void Hamming_XOR::Encode()
{
	//printf("encoding begin ...\t\n");
	for (int i = 0; i < StripeNum; i++) {
		for (int j = 0; j < Generate_Matrix.size(); j++) {
			for (int k = 0; k < Generate_Matrix[j].size(); k++) {
				xor_basic(buffer[i] + (Generate_Matrix[j][k] + 32) * UnitSize, buffer[i] + j * UnitSize, UnitSize);
			}
		}
	}
	memset(buffer[5], 0, sizeof(char) * CodeLen * UnitSize);
	for (int i = 0; i < StripeNum; i++) {
		xor_basic(buffer[5], buffer[i], CodeLen * UnitSize);
	}
	//printf("encoding end ...\t\n");
}

void Hamming_XOR::Decode()
{
	char *res = new char[UnitSize * ParityLen];					//һάָ�룬ָ��39����Ϣ�õ��İ���ʽ�ĵ�ַ;		//�洢��صİ���ʽ



	bitset<7> testParity(0);							//����ʽ��ȫΪ0�� ��Ҫһbitһ��bit��ȥ����7λ����ʽ��Ȼ��λ���������
	bitset<39> testInput(0);
	bitset<39> testOutput(0);

	for (int num = 0; num < StripeNum; num++) {							//��ÿһ��
		int flag = 0;
		memset(res, 0, sizeof(char) * UnitSize * ParityLen);
		for (int i = 0; i < Adjoint_Matrix.size(); i++) {
			for (int j = 0; j < Adjoint_Matrix[i].size(); j++) {
				xor_basic(res + Adjoint_Matrix[i][j] * UnitSize, buffer[num] + i * UnitSize, UnitSize);	//����д�Բ��ԣ�
			}
		}
		long long *lres = (long long *)res;
		long long **llbuffer = (long long **)buffer;
		//testParity.reset();
		//for (int m = 0; m < ParityLen; m++) {
		//	if (IsOne(res[0 + m * UnitSize], 0)) {
		//		testParity[ParityLen - m - 1] = !testParity[ParityLen - m - 1];
		//	}
		//}
		//cout << "testParity is " << testParity << endl;

		for (int i = 0; i < UnitSize / 8; i++) {						//ÿһ���ƫ��
			for (int j = 0; j < ParityLen; j++) {
				if (lres[i + j * (UnitSize / 8)]) {
					for (int k = 63; k >= 0; k--) {
						int flag = 0;
						if (IsOne(lres[i + j * (UnitSize / 8)], k)) {
							//std::cout << "pos:\t" << hex << (unsigned int)& lres[i + j * (UnitSize / 8)] << "\t" << k << endl;
							testParity.reset();
							for (int m = 0; m < ParityLen; m++) {
								if (IsOne(lres[i + m * (UnitSize / 8)], k)) {
									testParity[ParityLen - m - 1] = !testParity[ParityLen - m - 1];
								}
							}
							int result = isParityAllZero(testParity);				//�ж��ϲ��ĵõ��İ���ʽ�Ƿ�Ϊ0���鿴�䷵��ֵ�����˸�ֵ��0
							//cout << "testParity is " << testParity << endl;
							for (int m = 0; m < ParityLen; m++) {
								clrbit(lres[i + m * (UnitSize / 8)], k);
							}
							if (result == 0) {
								cout << "result == 0" << endl;
							}
							else if (result == 1) {
								//cout << "k " << k << endl;
								testInput.reset();
								testOutput.reset();
								for (int m = 0; m < CodeLen; m++) {
									if (IsOne(llbuffer[num][i + m * (UnitSize / 8)], k)) {
										testInput[CodeLen - m - 1] = !testInput[CodeLen - m - 1];
									}
								}
								//std::cout <<"pos:\t" <<hex << (unsigned int)& lres[i + j * (UnitSize / 8)] << endl;
								//cout << "testInput is " << testInput << endl;
								int high = (Array_Table[(int)(testParity.to_ulong())] >> 8) & 0xff;
								//cout << "high is " << high  << endl;
								if (high != 255) {
									testOutput = testInput.flip(38 - high);
								}
								flag = 0;													//�޴�
								cout << "the buffer " << num << " using inner-coding repair success." << endl;
								//cout << testOutput << endl;
								//cout << endl;
								//for (int m = 0; m < CodeLen; m++) {
								//	cout << testOutput[CodeLen - m - 1];
								//}
								//cout << endl;

								for (int m = 0; m < CodeLen; m++) {							//д����ȷ����Ϣ
									if (testOutput[CodeLen - m - 1]) { 						//��������ȷ�����Ϣӳ�䵽�ÿ��еĶ�Ӧ����
										setbit(llbuffer[num][i + m * (UnitSize / 8)], k);
									}
									else {
										clrbit(llbuffer[num][i + m * (UnitSize / 8)], k);
									}
								}
							}
							else if (result == -1) {
								cout << "enter the two-level decode" << endl;
								long long temp_input[39] = { 0 };
								long long temp_output[39] = { 0 };
								for (int m = 0; m < StripeNum + 1; m++) {
									if (m != num) {
										memset(temp_input, 0, sizeof(long long) * 39);
										for (int n = 0; n < CodeLen; n++) {
											if (IsOne(llbuffer[m][i + n * (UnitSize / 8)], k)) {
												//testGroupInput[m][CodeLen - 1 - n] = !testGroupInput[m][CodeLen - 1 - n];	//ȡͬ���������Ӧλ�õĺ���
												temp_input[n] = !temp_input[n];
											}
										}					
										int res2 = HammingDecode(temp_input);
										if (res2 == -1) {
											//cout << "the buffer " << num << " have many errors, and return." << endl;
											flag = 1;
											break;
										}
										else {
											for (int i = 0; i < 39; i++) {
												temp_output[i] ^= temp_input[i];
											}
										}
									}
								}
								if (!flag) {
									for (int m = 0; m < CodeLen; m++) {								//����޸��ɹ���д����ȷ����Ϣ
										if (temp_output[m]) { 										//��������ȷ�����Ϣӳ�䵽�ÿ��еĶ�Ӧ����
											setbit(llbuffer[num][i + m * (UnitSize / 8)], k);
										}
										else {
											clrbit(llbuffer[num][i + m * (UnitSize / 8)], k);
										}
									}
									cout << "the buffer " << num << " using two-level repair success." << endl;
									flag = 0;
								}
								else {
									cout << "the buffer " << num << " have many errors, and return." << endl;
								}
								/*vector<bitset<39> > testGroupInput(StripeNum + 1, bitset<39>(0));
								vector<bitset<39> > testGroupOutput(StripeNum + 1, bitset<39>(0));
								for (int m = 0; m < StripeNum + 1; m++) {
									if (m != num) {
										for (int n = 0; n < CodeLen; n++) {
											if (IsOne(llbuffer[m][i + n * (UnitSize / 8)], k)) {
												testGroupInput[m][CodeLen - 1 - n] = !testGroupInput[m][CodeLen - 1 - n];	//ȡͬ���������Ӧλ�õĺ���
											}
										}
									}
								}

								for (int m = 0; m < StripeNum + 1; m++) {
									if (m != num) {
										int res2 = HammingDecode(testGroupInput[m], testGroupOutput[m]); 						//ȡ���󣬶Ա�����������ƽ��н���
										if (res2 == -1) {
											//cout << "the buffer " << num <<" have many errors, and return." << endl;
											flag = 1;
											break;
										}
										else {
											testGroupOutput[0] ^= testGroupOutput[m];										//���ͬ�����Ϣ�ܹ���ȷ���룬�������Ӧ��Ϣ����
										}
									}
								}
								if (!flag) {
									for (int m = 0; m < CodeLen; m++) {													//����޸��ɹ���д����ȷ����Ϣ
										if (testGroupOutput[0][CodeLen - m - 1]) { 										//��������ȷ�����Ϣӳ�䵽�ÿ��еĶ�Ӧ����
											setbit(llbuffer[num][i + m * (UnitSize / 8)], k);
										}
										else {
											clrbit(llbuffer[num][i + m * (UnitSize / 8)], k);
										}
									}
									cout << "the buffer " << num << " using two-level repair success." << endl;
									flag = 0;
								}
								else {
									cout << "the buffer " << num << " have many errors, and return." << endl;
								}*/
							}
						}
						else {
							//cout << "lres is 0 " << k << " j is "<< j << endl;
						}
					}
				}
			}
		}
		if (!Check_Is_Right(before[num], buffer[num], CodeLen * UnitSize)) {
			cout << "Check_Is_Right is false, repair failed" << endl;//�жϻָ��Ƿ���ȷ
		}
		else {
			//cout << "the buffer " << num << " repair successfully!" << endl;
		}
	}

}



bool Hamming_XOR::Check_Is_Right(char * before, char * after, int size)
{
	long long * l_before = (long long *)before;
	long long * l_after = (long long *)after;
	for (int i = 0; i < size / 8; i++) {
		if (l_before[i] != l_after[i]) {				//�������ǰ�����ݲ���ȣ�֤���޸�ʧ�ܣ����ؼ���
			cout << "it has error,repair failed" << endl;
			return false;
		}
	}
	return true;
}

void Hamming_XOR::testCode()
{


	getTable();														//����

	struct timeval st, en;											//��ʱ
	long timediff_write = 0;										//��������ʱ��
	long timediff_read = 0;											//��������ʱ��

	for (int i = 0; i < 1; i++) {

		gettimeofday(&st, NULL);
		Encode();													//����
		gettimeofday(&en, NULL);
		timediff_write += timeDiffMacroSeconds(st, en);				//��ɱ������

		before = new char*[StripeNum + 1];							//δע�����֮ǰ�����ע����������޸����������бȽϣ����Ƿ�ָ���
		for (int i = 0; i < StripeNum + 1; i++) {					//���г�ʼ����һЩ����
			before[i] = new char[CodeLen * UnitSize];
			for (int j = 0; j < CodeLen * UnitSize; j++) {
				before[i][j] = buffer[i][j];
			}
		}

		reversebit(buffer[0][0], 0); 								//ע����󣬵�һ��ĸ�λ����λ��ת
		reversebit(buffer[0][UnitSize * 1], 0); 					//ע����󣬵ڶ���ĸ�λ����λ��ת

		reversebit(buffer[1][0], 0); 								//ע����󣬵�һ��ĸ�λ����λ��ת
		//reversebit(buffer[1][UnitSize * 1], 0); 					//ע����󣬵ڶ���ĸ�λ����λ��ת
		//															//
		//reversebit(buffer[0][0], 7); 								//ע����󣬵�һ��ĸ�λ����λ��ת
		//reversebit(buffer[0][UnitSize * 1], 7); 					//ע����󣬵ڶ���ĸ�λ����λ��ת
		//															//
		//reversebit(buffer[1][0], 7); 								//ע����󣬵�����ĸ�λ����λ��ת
		//reversebit(buffer[1][UnitSize * 1], 7); 					//ע����󣬵ڶ���ĸ�λ����λ��ת

		gettimeofday(&st, NULL);
		Decode();
		gettimeofday(&en, NULL);
		timediff_read += timeDiffMacroSeconds(st, en);


	}


	printf("write time cost %s, %ld us\n", timeFormatMacroToString(timediff_write), timediff_write);
	printf("read time cost %s, %ld us\n", timeFormatMacroToString(timediff_read), timediff_read);

}


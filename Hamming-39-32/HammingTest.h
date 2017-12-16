#ifndef HammingTest_h
#define HammingTest_h

#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

const int dataLen = 32;
const int parityLen = 7;
const int codeLen = 39;
const int blocksize = 39;

class HammingTest
{
public:
    void HammingEncode(bitset<dataLen> &input, bitset<codeLen> &output);
    void HammingDecode(bitset<codeLen> &input, bitset<codeLen> &output);
    void getParityXOR(bitset<codeLen> &input, bitset<parityLen> &output);
    void setTable();
    void getTable();
    void testHamming();
};


#endif // HammingTest

#ifndef BCHCode_h
#define BCHCode_h
#ifndef _CLOCK_T_DEFINED
typedef long clock_t;
#define _CLOCK_T_DEFINED
#endif

#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <bitset>
#include <unordered_map>
#include <time.h>

using namespace std;

const int dataLen = 32;
const int parityLen = 12;
const int codeLen = 44;
const int blocksize = 44;
const int stripesize = 4; //实行4个编码条纹一组来进行整体的异或编码

class BCHCode {
public:
    void BCHEncode(bitset<dataLen> & input, bitset<codeLen> & output);
    void BCHDecode(bitset<codeLen> & input, bitset<codeLen> & output);
    void getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output);
    void setTable();
    void getTable();
    void testBCH();
    void testOneorTwoError();
    void errTest();
};

#endif /* BCHCode_h */



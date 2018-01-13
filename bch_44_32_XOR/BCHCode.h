#ifndef BCHCode_h
#define BCHCode_h

#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <bitset>
#include <unordered_map>

using namespace std;

const int dataLen = 32;
const int parityLen = 12;
const int codeLen = 44;
const int blocksize = 44;

class BCHCode {
public:
    void BCHEncode(bitset<dataLen> & input, bitset<codeLen> & output);
    void BCHDecode(bitset<codeLen> & input, bitset<codeLen> & output);
    void getParityXOR(bitset<codeLen> & input, bitset<parityLen> & output);
    void setTable();
    void getTable();
    void testBCH();
    void errTest();
};

#endif /* BCHCode_h */



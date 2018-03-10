#include <iostream>
#include "BCHCode.h"
#include <sys/time.h>
using namespace std;


unsigned long Total_time = 0;
int main() {
    const int dataLen = 32;
    int array_RulesGenenate[dataLen] = {
        0b001110010101,
        0b011100101010,
        0b111001010100,
        0b111100111101,
        0b110111101111,
        0b100001001011,
        0b001100000011,
        0b011000000110,
        0b110000001100,
        0b101110001101,
        0b010010001111,
        0b100100011110,
        0b000110101001,
        0b001101010010,
        0b011010100100,
        0b110101001000,
        0b100100000101,
        0b000110011111,
        0b001100111110,
        0b011001111100,
        0b110011111000,
        0b101001100101,
        0b011101011111,
        0b111010111110,
        0b111011101001,
        0b111001000111,
        0b111100011011,
        0b110110100011,
        0b100011010011,
        0b001000110011,
        0b010001100110,
        0b100011001100
    };
    struct  timeval  start_time;
    struct  timeval  end_time;
    bitset<dataLen> enInput("00001100000000000000000000000011");
    bitset<codeLen> enOutput;
    gettimeofday(&start_time, NULL);
    int res = 0b0;
    for(int i =  0;  i< 1000; i++) {
        for(int i = 0; i < dataLen; i++) {
            if(enInput[i]) {
                res ^= array_RulesGenenate[i];
            }
        }
        enOutput = res;
        //进行赋值操作
        for(int i = 0; i < dataLen; i++) {
            enOutput[i + 12] = enInput[i];
        }
    }

    //struct  timeval  start_time;
    //struct  timeval  end_time;

    //BCHCode *bchCode = new BCHCode();
    //bchCode -> testBCH();
    gettimeofday(&end_time, NULL);
    Total_time += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
    cout << "\tTotal operation Time is " << (double)Total_time  / 1000 << " ms" << endl;
    //bchCode -> errTest();
    return 0;
}



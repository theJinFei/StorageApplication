//
//  main.cpp
//  BCH(44, 32)
//
//  Created by Shuaibing_Zhao on 2017/11/8.
//  Copyright © 2017年 Shuaibing_Zhao. All rights reserved.
//

#include <iostream>
#include "BCHCode.h"

using namespace std;

const int dataLen = 32;
//const int parityLen = 12;
const int codeLen = 44;

int main(int argc, const char * argv[]) {
    // insert code here...
    int encodeInput[dataLen] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int encodeOutput[codeLen];
    
    for(int i = 0; i < dataLen; i++) {
        printf("%-2d", encodeInput[i]);
    }
    printf("\n");
    
    BCHCode * bch = new BCHCode();
    bch->BCHEncode(encodeInput, encodeOutput);
    
    for(int i = 0; i < codeLen; i++) {
        printf("%-2d", encodeOutput[i]);
    }
    printf("\n");
    
    return 0;
}

//
//  BCHCode.hpp
//  BCH(44, 32)
//
//  Created by Shuaibing_Zhao on 2017/11/8.
//  Copyright © 2017年 Shuaibing_Zhao. All rights reserved.
//

#ifndef BCHCode_h
#define BCHCode_h

#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

class BCHCode {
public:
    void BCHEncode(int input[], int output[]);
    void BCHDecode(int input[], int output[]);
};

#endif /* BCHCode_h */



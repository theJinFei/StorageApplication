#include <iostream>
#include "BCHCode.h"
using namespace std;



int main()
{
    BCHCode *bchCode = new BCHCode();
    bchCode -> testBCH();
    bchCode -> errTest();
    return 0;
}

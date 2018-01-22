#include <iostream>
#include "BCHCode.h"
using namespace std;


unsigned long Total_time = 0;
int main()
{
    clock_t start, finish;
    start = clock();
    BCHCode *bchCode = new BCHCode();
    bchCode -> testBCH();
    finish = clock();
    Total_time =  (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "\tTotal operation Time is " << Total_time << " s" << endl;
    //bchCode -> errTest();
    return 0;
}

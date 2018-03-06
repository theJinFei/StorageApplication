#include <iostream>
#include "BCHCode.h"
//#include <sys/time.h>
using namespace std;


unsigned long Total_time = 0;
int main()
{
	//struct  timeval  start_time;
	//struct  timeval  end_time;
	//gettimeofday(&start_time, NULL);
	BCHCode *bchCode = new BCHCode();
	bchCode->testBCH();
	//gettimeofday(&end_time, NULL);
	//Total_time += 1000000 * (end_time.tv_sec-start_time.tv_sec)+ end_time.tv_usec-start_time.tv_usec;
	//cout << "\tTotal operation Time is " << (double)Total_time  / 1000000 << " s" << endl;
	//bchCode -> errTest();
	//system("pause");

	return 0;
}



#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <bitset>
#pragma warning( disable : 4996 )

using namespace std;
#pragma comment (lib,"OpenCL.lib")

#define dataLen 32
#define	parithyLen 7
#define codeLen 39


//把文本文件读入一个string中
int convertToString(const char *filename, std::string& s)
{
	size_t size;
	char*  str;

	std::fstream f(filename, (std::fstream::in | std::fstream::binary));

	if (f.is_open())
	{
		size_t fileSize;
		f.seekg(0, std::fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, std::fstream::beg);

		str = new char[size + 1];
		if (!str)
		{
			f.close();
			return NULL;
		}

		f.read(str, fileSize);
		f.close();
		str[size] = '\0';

		s = str;
		delete[] str;
		return 0;
	}
	printf("Error: Failed to open file %s\n", filename);
	return 1;
}

int main(int argc, char* argv[])
{
	

	// 27 26
	//初始化矩阵
	int i, j, k;
	int Array_Input[dataLen] = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1};
	int Array_Generate[dataLen * codeLen] = 
	 {   1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1 ,
		 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0 ,
		 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1 ,
		 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1 ,
		 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0 ,
		 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1 ,
		 0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0 ,
		 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0 ,
		 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0 ,
		 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0 ,
		 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,1,1,1 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1 };

	//int * src1 = (int*)malloc(dataLen * codeLen * sizeof(int));
	//src1 = Array_src1;
	//int *src2 = (int*)malloc(dataLen * sizeof(int));
	//for (i = 0; i < dataLen; i++)
	//	src2[i] = rand() % 2;
	//int *outbuf = 0;
	//outbuf = (int*)malloc(codeLen * sizeof(int));

	//srand((unsigned)time(NULL) + 1000);
	int res[codeLen] = {0};
	int *outbuf = (int*)malloc(codeLen * sizeof(int));
	int Ndim = 1;
	int Pdim = 32;
	int Mdim = 44;
	
	for (int i = 0; i < codeLen; i++) {
		int tmp = 0;
		for (int j = 0; j < dataLen; j++) {
			tmp += Array_Input[j] * Array_Generate[j * codeLen + i];
		}
		if (tmp % 2 == 0) {
			res[codeLen - 1 - i] = 0;
		}
		else {
			res[codeLen - 1-  i] = 1;
		}
	}
	for (int i = 0; i < codeLen; i++) {
		cout << res[codeLen - 1 - i];
	}
	cout << endl;
	bitset<dataLen> enInput("00001100000000000000000000000011");
	bitset<codeLen> enOutput;
	for (int j = 0; j < codeLen; j++) {
		for (int i = 0; i < dataLen; i++) {
			if (enInput[31 - i] * Array_Generate[i * codeLen + j]) {
				enOutput.flip(38 - j);
			}
		}
	}
	cout << enOutput << endl;

	/*int a[32][44] = {
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1 },
		{ 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0 },
		{ 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1 },
		{ 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1 },
		{ 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0 },
		{ 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1 },
		{ 0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0 },
		{ 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0 },
		{ 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0 },
		{ 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,1,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1 }
	};
	bitset<dataLen> enInput("00001100000000000000000000000000");
	bitset<codeLen> enOutput;
	int(*pArr)[44] = a;
	for (int j = 0; j < codeLen; j++) {
		for (int i = 0; i < dataLen; i++) {
			if (enInput[31 - i] * a[i][j]) {
				enOutput.flip(38 - j);
			}
		}
	}*/
	//cout << "In CPU，the HammingEncode result is  " << enOutput << endl;
	//for (int i = 0; i < codeLen; i++) {
	//	for (int j = 0; j < dataLen; j++) {
	//		cout << pArr[i][j] << " ";
	//	}
	//	cout << endl;
	//}


	cl_uint status;
	cl_platform_id platform;

	//创建平台对象
	status = clGetPlatformIDs(1, &platform, NULL);

	cl_device_id device;

	//创建GPU设备
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU,
		1,
		&device,
		NULL);
	//创建context
	cl_context context = clCreateContext(NULL,
		1,
		&device,
		NULL, NULL, NULL);
	//创建命令队列
	cl_command_queue queue = clCreateCommandQueue(context,
		device,
		CL_QUEUE_PROFILING_ENABLE, NULL);

	//创建三个OpenCL内存对象
	cl_mem clsrc1 = clCreateBuffer(context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		dataLen * codeLen * sizeof(cl_int), Array_Generate,
		NULL);
	cl_mem clsrc2 = clCreateBuffer(context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		dataLen * sizeof(cl_int), Array_Input,
		NULL);

	cl_mem cloutbuf = clCreateBuffer(context,
		CL_MEM_WRITE_ONLY,
		codeLen * sizeof(cl_int),
		NULL, NULL);

	const char * filename = "mulmatrix.cl";
	std::string  sourceStr;
	status = convertToString(filename, sourceStr);
	const char * source = sourceStr.c_str();
	size_t sourceSize[] = { strlen(source) };

	//创建程序对象
	cl_program program = clCreateProgramWithSource(
		context,
		1,
		&source,
		sourceSize,
		NULL);
	//编译程序对象
	status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if (status != 0)
	{
		printf("clBuild failed:%d\n", status);
		char tbuf[0x10000];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0x10000, tbuf, NULL);
		printf("\n%s\n", tbuf);
		return -1;
	}
	double total = 0;
	
		//创建Kernel对象
		cl_kernel kernel = clCreateKernel(program, "simpleMultiply", NULL);

		//设置Kernel参数
		clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cloutbuf);
		clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&clsrc1);
		clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&clsrc2);


		//Set local and global workgroup sizes
		//size_t localws[2] = { 8, 8 };
		//size_t globalws[2] = { M,N };

		cl_event ev;
		size_t maxcodeLen = codeLen;
		for (int i = 0; i < 1000; i++) {
			//执行kernel
			clEnqueueNDRangeKernel(
				queue, kernel,
				1, 0, &maxcodeLen, NULL,
				0, NULL, &ev);
			clFinish(queue);

			//计算kerenl执行时间 
			cl_ulong startTime, endTime;
			clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_START,
				sizeof(cl_ulong), &startTime, NULL);
			clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_END,
				sizeof(cl_ulong), &endTime, NULL);
			cl_ulong kernelExecTimeNs = endTime - startTime;
			//printf("kernal exec time :%8.6f ms\n ", kernelExecTimeNs*1e-6);
			total += kernelExecTimeNs*1e-6;
		}
		cout << total << "ms" << endl;
		
		
	


	cl_int *op_data = 0;
	//数据拷回host内存
	// copy results from device back to host
	op_data = (cl_int *)clEnqueueMapBuffer(queue,
		cloutbuf,
		CL_TRUE,
		CL_MAP_READ,
		0,
		codeLen * sizeof(cl_int),
		0, NULL, NULL, NULL);

	if (op_data != NULL) {
		for (int i = codeLen - 1; i >=  0; i--) {
			//cout << *(op_data + i);
		}
	}
	//cout << endl;

	//验证GPU计算结果
	//for (i = 0; i < M*N; i++)
	//{
	//	//printf("%d, %6.3f,%6.3f\n",i,outbuf[i],op_data[i]);
	//	if (abs(outbuf[i] - op_data[i]) > 0.0001)
	//	{
	//		printf("check failed\n");
	//		break;
	//	}
	//}
	//if (i == M*N)
	//	printf("check passed\n");

	////创建Kernel对象
	//kernel = clCreateKernel(program, "simpleMultiplyCol", NULL);

	////设置Kernel参数
	//clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cloutbuf);
	//clSetKernelArg(kernel, 1, sizeof(cl_int), (void *)&dimx);
	//clSetKernelArg(kernel, 2, sizeof(cl_int), (void *)&dimy);
	//clSetKernelArg(kernel, 3, sizeof(cl_int), (void *)&dimc);
	//clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&clsrc1);
	//clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&clsrc2);


	////Set local and global workgroup sizes
	//size_t globalws1[2] = { N,M };

	////执行kernel
	//clEnqueueNDRangeKernel(
	//	queue, kernel,
	//	2, 0, globalws1, localws,
	//	0, NULL, &ev);

	//clFinish(queue);
	////计算kerenl执行时间 
	//clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_START,
	//	sizeof(cl_ulong), &startTime, NULL);
	//clGetEventProfilingInfo(ev, CL_PROFILING_COMMAND_END,
	//	sizeof(cl_ulong), &endTime, NULL);
	//kernelExecTimeNs = endTime - startTime;
	//printf("col kernal exec time :%8.6f ms\n ", kernelExecTimeNs*1e-6);


	////数据拷回host内存
	//// copy results from device back to host
	//op_data = (cl_int *)clEnqueueMapBuffer(queue,
	//	cloutbuf,
	//	CL_TRUE,
	//	CL_MAP_READ,
	//	0,
	//	M*N * sizeof(cl_int),
	//	0, NULL, NULL, NULL);

	////验证GPU计算结果
	//for (i = 0; i < M*N; i++)
	//{
	//	//printf("%d, %6.3f,%6.3f\n",i,outbuf[i],op_data[i]);
	//	if (abs(outbuf[i] - op_data[i]) > 0.0001)
	//	{
	//		printf("check failed\n");
	//		break;
	//	}
	//}
	//if (i == M*N)
	//	printf("check passed\n");


	//if (src1)
	//	free(src1);
	//if (src2)
	//	free(src2);
	//if (outbuf)
	//	free(outbuf);


	////删除OpenCL资源对象
	//clReleaseMemObject(clsrc1);
	//clReleaseMemObject(clsrc2);
	//clReleaseMemObject(cloutbuf);
	//clReleaseProgram(program);
	//clReleaseCommandQueue(queue);
	//clReleaseContext(context);
	return 0;
}


#pragma OPENCL EXTENSION cl_amd_printf : enable
__kernel void simpleMultiply( 
		__global int* c,
		__global int* a, __global int* b)
 {
	//printf("a[i]");
	//for(int i = 0; i < 32; i++){
	//	printf("%d", a[i]);
	//}
	//for(int i = 0; i < 44; i++){
	//	printf("b [%d] is %d", i, b[i]);
	//}
	int id = get_global_id(0);
	int codeLen = 39;
	int tmp = 0;
	for (int j = 0; j < 32; j++) {
		tmp += b[j] * a[j * codeLen + id];
	}
	if (tmp % 2 == 0) {
		c[codeLen - 1 - id] = 0;
	}
	else {
		c[codeLen - 1-  id] = 1;
	}
}


__kernel void simpleMultiplyCol( 
		__global int* c, int Wa, int Wb, int Wc,
		__global int* a, __global int* b)
 {

	//Get global position in Y direction
	int row = get_global_id(0);
	//Get global position in X direction
	int col   = get_global_id(1); 
	int sum = 0.0f; 
	//Calculate result of one element
	for (int i = 0; i < Wc; i++)
	 {
		sum += 
                     a[col*Wc+i] * b[i*Wb+row];
	}
	c[col*Wb+row] = sum;
}


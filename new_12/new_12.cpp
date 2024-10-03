#include "stdafx.h"
/*
练习题：			
			
1、*() 与 []是可以互换的，也就是说：			
			
*(*(p+1)+2) 相当于 p[1][2]			 
			
那*(p+1)[2] 是否一定等于p[1][2]呢？ 通过反汇编进行论证。			
			
2、使用数组指针遍历一个一维数组.			
*/

int main(int argc, char* argv[])
{
	int Arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int (*pArr)[2] = (int (*)[2])&Arr;
	printf ("%d\n%d\n%d\n%d\n%d\n%d\n", \
		pArr[1][2], \
		*(*(pArr + 1) + 2), \
		(*(pArr + 1))[2],
		*(pArr + 1)[2], \
		*(*((pArr + 1) + 2)),\
		*(*(pArr + 1 + 2)));
	
	// 使用数组指针遍历一个一维数组
	for (int i = 0; i < 10; i++)
	{
		printf("%d\t", *(*(pArr + 0) + i));
		// printf("%d\t", pArr[0][i]);
	}
	return 0;
}
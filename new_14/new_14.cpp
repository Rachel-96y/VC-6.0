#include "stdafx.h"
#include <stdlib.h>
#include <memory.h>

char* function(void)
{
	/* 使用malloc在堆中动态申请内存 */

	int* pMemory = (int*)malloc(sizeof(int)*128);

	if (pMemory == NULL)  // 判断是否堆空间申请成功
	{
		exit(0);  // 参数为0代表正确退出
	}

	void* pMemset = memset(pMemory, 0xCCCCCCCC, sizeof(int)*128);  // 初始化要使用的空间

	char* pArrayFirstMemory = (char*)(pMemory + 5);  // 声明一个char*类型的指针 指向从pMemory向后20个int的位置

	char (*pArray)[2] = (char (*)[2])pArrayFirstMemory;  // 声明数组指针 值为pArrayFirstMemory

	for (int i = 0; i < 10; i++)
	{
		*(pArrayFirstMemory + i) = i;   // 为数组赋值  0 - 9
	}

	*(pArrayFirstMemory + i) = 0x00;  // 结束标志

	i = 0; // 初始化i

	do
	{
		printf("%d\n", *(*(pArray + 0) + i++));
	}
	while (*(*(pArray + 0) + i));

	free(pMemory);  // 释放空间

	pMemory = NULL;  //将指针置为 NULL

	return pArrayFirstMemory;
}

int main(int argc, char* argv[])
{
	char* pArrayFirstMemory = function();

	return 0;
}
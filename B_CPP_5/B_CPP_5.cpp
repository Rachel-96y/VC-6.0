// B_CPP_5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

LPVOID Test(int whoami)
{
	MS MyStr_1(200);

	MyStr_1.TestFileBuffer();
	MyStr_1.SetString("MyCode_1");
	MyStr_1.PrintString();
	MyStr_1.AppendString("HelloWorld");
	MyStr_1.PrintString();
	int result_1 = MyStr_1.Size();
	printf("%d\n", result_1);

	MS MyStr_2;

	MyStr_2.TestFileBuffer();
	MyStr_2.SetString("MyCode_2");
	MyStr_2.PrintString();
	MyStr_2.AppendString("HelloWorld");
	MyStr_2.PrintString();
	int result_2 = MyStr_2.Size();
	printf("%d\n", result_2);

	if (!whoami)
	{
		LPVOID result = MyStr_1.RETN_VALUE();
		return result;
	}
	else
	{
		LPVOID result = MyStr_1.RETN_VALUE();
		return result;
	}	
}

int main(int argc, char* argv[])
{
	/*
		本节练习：
		将上一节课的所有练习改为class实现
		1、添加private/public进行权限控制.
		2、将类的定义与实现分开来写：定义写到xxx.h中 函数实现写在xxx.cpp中
	*/

	LPVOID pFileBuffer = Test(0);
	if (*(LPDWORD)pFileBuffer == 0xDDDDDDDD)
	{
		printf("返回了 MyStr_1.pFileBuffer / MyStr_1的析构函数执行成功!\n*(LPDWORD)pFileBuffer: %x\n", *(LPDWORD)pFileBuffer);
	}
	else
	{
		printf("返回了 MyStr_2.pFileBuffer /  \n *(LPDWORD)pFileBuffer的值是: %x\n", *(LPDWORD)pFileBuffer);
	}

	getchar();
	
	return 0;
}

// X_Main_1.cpp : Defines the entry point for the console application.
//



/*
本节练习		
		
1、创建一个静态链接库，并在代码中使用.		
		
2、创建一个动态链接库，使用两种方式进行导出(_declspec(dllexport)与.def文件).		
		
3、分别使用隐式链接和显示链接使用一个DLL文件.		
*/



/*
#include "stdafx.h"

#pragma comment(lib, "Static_Library_test_1.lib")

int main(int argc, char* argv[])

{	// 静态链接库使用
	int a = function(1, 2, 3);
	printf("%d\n", a);
	getchar();
	return 0;
}
*/



/*
#include "stdafx.h"

extern "C" __declspec(dllimport) int __stdcall function(int x, int y, int z);

#pragma comment(lib, "Dynamic_Link.lib")

int main(int argc, char* argv[])

{	// 动态链接库隐式链接
	int a = function(1, 2, 3);
	printf("%d\n", a);
	getchar();
	return 0;
}
*/




#include "stdafx.h"

int main(int argc, char* argv[])
{
	// 动态链接库显式链接 

	// 将需要使用的模块载入当前EXE的内存
	HMODULE Hmodule = LoadLibrary("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\Dynamic_Link_2.dll");

	// 动态获取需要使用的函数的当前地址
	FARPROC FunctionAddress = GetProcAddress(Hmodule, "Sub");

	// 定义函数指针并赋值
	int (__stdcall *pFunction)(int, int) = (int (__stdcall *)(int, int))FunctionAddress;

	int result = pFunction(5, 2);
	printf("%d\n", result);
	getchar();
	return 0;
}


// Win32Api_1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	// setlocale(LC_ALL,"");

	// DbgPrintf("Hello\n");

	// HWND result =  FindWindow("Program Manager", 0);

	// DbgPrintf("%x\n", result);

	// MessageBox(0, 0, 0, 0);

	// DWORD Erro = ::GetLastError();

	// DbgPrintf("%d\n", Erro);


	/*
		本节练习：										
		1、分别使用 wchar_t / wprintf / wcslen / wcscpy / wcscat / wcscmp / wcsstr						
		写一个例子.					
		2、查MSDN 了解WinMain其他3个参数的意义						
	*/
	
	TCHAR name[] = TEXT("美国");
	DbgPrintf("%s\n", name);

	wprintf(L"美国\n");  // 需要在控制台输出

	DWORD result = wcslen(L"我爱aa美国");
	DbgPrintf("%d\n", result);
	
	wchar_t str1[] = L"我爱aa美国";
	CONST WCHAR str2[] = L"123";
	wcscpy(str1, str2);
	DbgPrintf("%s\n", str1);

	wchar_t str3[] = L"3125671";
	wchar_t str4[] = L"567";
	// wcscat(str3, str4);
	int ret = wcscmp(str3, str4);
	if (!ret)
	{
		DbgPrintf("相等!");
	}

	wchar_t * answer = wcsstr(str3, str4);
	DbgPrintf("%d\n", *answer);

	// 2、查MSDN 了解WinMain其他3个参数的意义		
	// hPrevInstance	应用程序的前一个实例的句柄。对于一个基于Win32的应用程序，这个参数总是为空。
	// lpCmdLine		指向一个空尾的字符串，指定应用程序的命令行，不包括程序名称。要检索整个命令行，请使用GetCommandLine函数。
	// nCmdShow			指定窗口的显示方式

	return 0;
}


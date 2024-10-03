// Win32Api_15.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

// 以下对TLS表操作的代码在VC++6.0中不被支持 需要使用更高版本 如VS2010

// 告诉链接器,将要使用TLS表
#pragma comment(linker, "/INCLUDE:__tls_used")


// TLS表回调函数(正常情况下,一个TLS的回调函数会被执行3次)
VOID NTAPI TLS_CALLBACK1(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// 第一个TLS回调函数
	if (Reason == DLL_PROCESS_ATTACH)
	{
		MessageBox(0, "第一个TLS回调函数执行第一次", 0, 0);
	}
	if (Reason == DLL_THREAD_ATTACH)
	{
		MessageBox(0, "第一个TLS回调函数执行第二次", 0, 0);
	}
	if (Reason == DLL_THREAD_DETACH)
	{
		MessageBox(0, "第一个TLS回调函数执行第三次", 0, 0);
	}
	if (Reason == DLL_PROCESS_DETACH)
	{
		MessageBox(0, "第一个TLS回调函数执行第四次", 0, 0);
	}
}

VOID NTAPI TLS_CALLBACK2(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		// 第二个TLS回调函数
		MessageBox(0, "第二个TLS回调函数执行第一次", 0, 0);
	}
	if (Reason == DLL_THREAD_ATTACH)
	{
		MessageBox(0, "第二个TLS回调函数执行第二次", 0, 0);
	}
	if (Reason == DLL_THREAD_DETACH)
	{
		MessageBox(0, "第二个TLS回调函数执行第三次", 0, 0);
	}
	if (Reason == DLL_PROCESS_DETACH)
	{
		MessageBox(0, "第二个TLS回调函数执行第四次", 0, 0);
	}
}


// 存储TLS表数据回调函数
#pragma data_seg(".CRT$XLX")
PIMAGE_TLS_CALLBACK pIMAGE_TLS_CALLBACKs[] = {TLS_CALLBACK1, TLS_CALLBACK2, NULL};
#pragma data_seg()


typedef struct _FUNC{
	_FUNC()
	{
		MessageBox(0, "全局变量", 0, 0);
	}
}FUNC, *PFUNC;

FUNC flag;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	
	MessageBox(0, "主函数", 0, 0);
	return 0;
}
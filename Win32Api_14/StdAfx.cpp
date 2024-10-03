// stdafx.cpp : source file that includes just the standard includes
//	Win32Api_14.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

//////////////////////////////////////////////////////////////////////
void WINAPIV OutputDebugStringF(const char *format, ...)
{
	/*
		这是调试相关的函数 只在 _DEBUG 模式有作用
	*/
    va_list vlArgs;
    char    *strBuffer = (char*)GlobalAlloc(GPTR, 4096);
    va_start(vlArgs, format);
    _vsnprintf(strBuffer, 4096 - 1, format, vlArgs);
    va_end(vlArgs);  
    strcat(strBuffer, "\n");
    OutputDebugStringA(strBuffer);
    GlobalFree(strBuffer);

    return;
}

//////////////////////////////////////////////////////////////////////
HANDLE WINAPI CheckIsExsist(void)
{
	/*
		检测是否当前程序仅被启动了一次
		如果函数执行成功则返回一个内核对象句柄
	*/

	// 获取内核对象中此程序独特的标志
	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, "CHECK_EXSIST_WIN32API_FLAG");

	// 用于存储新创建的内核句柄
	HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = NULL;
	
	// 值为0则代表是第一次运行,即OpenMutex执行失败
	if (!g_hMutex)
	{
		// 如果此程序是第一次运行则创建内核对象用作全局标志
		g_hCHECK_EXSIST_WIN32API_FLAG = ::CreateMutex(NULL, FALSE, "CHECK_EXSIST_WIN32API_FLAG");
		if (!g_hCHECK_EXSIST_WIN32API_FLAG)
		{
			_exit(0);
		}
	}
	else
	{
		// 如果此程序已经执行则直接退出
		_exit(0);
	}

	// 返回内核对象句柄
	return g_hCHECK_EXSIST_WIN32API_FLAG;
}

//////////////////////////////////////////////////////////////////////
// stdafx.cpp : source file that includes just the standard includes
//	Win32Api_9.pch will be the pre-compiled header
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

VOID WINAPI CheckIsExsist(void)
{
	/*
		此函数通过遍历进程 简单判断自身是否多开
	*/
	
	// 如果成功返回进程快照指针
	LPVOID pSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// 给 PROCESSENTRY32 的 dwSize 赋值
	PROCESSENTRY32 StructProcessEntry32 = {0};
	StructProcessEntry32.dwSize = sizeof(PROCESSENTRY32);

	// 给 MODULEENTRY32 的 dwSize 赋值
	MODULEENTRY32 StructModuleEntry32 = {0};
	StructModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	// 获取第一个进程 放进 StructProcessEntry32.szExeFile 中
	Process32First(pSnapshot, &StructProcessEntry32);

	StructModuleEntry32.th32ProcessID = 0;

	// 如果成功返回模块快照指针
	HANDLE pModSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, StructProcessEntry32.th32ProcessID);

	// 获取第一个模块 放进 StructModuleEntry32.szExeFile 中
	Module32First(pModSnapshot, &StructModuleEntry32);

	// 得到当前进程名称
	PTCHAR szBaseName = StructModuleEntry32.szModule;

	// 计数 用于判断此程序(有此名字的exe) 是否是唯一的
	DWORD dwCount = 0;
	do
	{
		// 比较两个字符串是否相等
		DWORD dwResult = strcmp(szBaseName, StructProcessEntry32.szExeFile);
		if (!dwResult)
		{
			dwCount++;
		}
	}
	// 当没有任何进程时跳出循环
	while(Process32Next(pSnapshot, &StructProcessEntry32));

	if (dwCount == 1)
	{
		// 如果只有一个进程则继续执行
		return;
	}
	else
	{
		// 还有另一个进程则直接退出
		exit(0);
	}
}
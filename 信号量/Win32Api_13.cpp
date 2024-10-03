// Win32Api_13.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

// 信号量标志
HANDLE g_hSemaphore = {0};

// 用于存储编辑框句柄
HWND hEditAll[0x3] = {0};

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcAll(LPVOID lpParameter)
{
	// 子线程处理函数
	
	// 转换需要的数组值的下标
	DWORD dwIndex = (DWORD)lpParameter;

	// 存储需要转换的整形和字符串
	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};

	// 接收到信号后将信号量-1
	WaitForSingleObject(g_hSemaphore, INFINITE);
	do
	{
		// 获取文本框中的值
		SendMessage(hEditAll[dwIndex], WM_GETTEXT, 0x10, (LPARAM)szDATA);

		// 转换并且修改
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		
		// 转换并写回
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEditAll[dwIndex], WM_SETTEXT, 0, (LPARAM)szDATA);

		// 睡眠10毫秒用于观测
		Sleep(10);
	}
	while(dwNumber != 1000);

	// 将信号量+1
	ReleaseSemaphore(g_hSemaphore, 1, NULL);

	return dwIndex;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// 主任务线程

	// 创建信号量内核对象
	g_hSemaphore = CreateSemaphore(NULL, 0, 3, NULL);

	// 用于存储线程内核对象句柄
	HANDLE hThreadArrays[0x3] = {0};

	// 创建3个线程对象
	hThreadArrays[0] = ::CreateThread(NULL, 0, ThreadProcAll, (LPVOID)0, 0, NULL);
	hThreadArrays[1] = ::CreateThread(NULL, 0, ThreadProcAll, (LPVOID)1, 0, NULL);
	hThreadArrays[2] = ::CreateThread(NULL, 0, ThreadProcAll, (LPVOID)2, 0, NULL);

	// 获取第一个编辑框的句柄
	HWND hEdit_1 = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);

	// 设置第一个编辑框的信息(模拟)
	Sleep(1000);
	SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)"200");
	Sleep(1000);
	SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)"500");
	Sleep(1000);
	SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)"1000");
	Sleep(1000);

	// 释放信号(这里释放2个)
	ReleaseSemaphore(g_hSemaphore, 2, NULL);

	// 等待全部线程执行完毕
	::WaitForMultipleObjects(3, hThreadArrays, TRUE, INFINITE);

	// 释放线程内核对象空间
	::CloseHandle(hThreadArrays[0]);
	::CloseHandle(hThreadArrays[1]);
	::CloseHandle(hThreadArrays[2]);

	// 释放信号量内核对象
	::CloseHandle(g_hSemaphore);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	// 用于处理"IDD_DIALOG_MAIN"对话框的回调函数
	// 主线程

	switch(uMsg)								
	{
	case WM_CLOSE:
		{
			// 按下退出按钮
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// 为编辑框句柄数组赋值
			hEditAll[0] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_2);
			hEditAll[1] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_3);
			hEditAll[2] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_4);

			// 界面初始化
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(hEditAll[0], WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(hEditAll[1], WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(hEditAll[2], WM_SETTEXT, 0, (LPARAM)"0");
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_1:
				{
					// 启动主任务线程
					HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			} // End switch
		return CALLBACK_COMPLETED;
		} 
	}// End switch
	return CALLBACK_UNCOMPLETED;
}

//////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// 线程同步

	// 利用互斥体防止多开
	HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = CheckIsExsist();

	// 调用对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);

	// 当程序将要结束前关闭内核句柄
	::CloseHandle(g_hCHECK_EXSIST_WIN32API_FLAG);

	return 0;
}
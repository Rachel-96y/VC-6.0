// Win32Api_11.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

BYTE FLAG = 0x00;
CRITICAL_SECTION csCriticalSection = {0};

/*
//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_1(LPVOID lpParameter)
{
	// 第一个线程

	// 获取互斥体标志
	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS,FALSE, "g_hWIN32API_THREAD_FLAG");

	// 存储需要转换的整形和字符串
	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};

	// 得到目标编辑框和自身编辑框的句柄
	HWND hEdit_1 = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	HWND hEdit_2 = GetDlgItem((HWND)lpParameter, IDC_EDIT_2);
	do
	{
		// 监听互斥体是否被释放
		::WaitForSingleObject(g_hMutex,INFINITE);

		// 减目标编辑框中的值
		SendMessage(hEdit_1, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (dwNumber < 50)
		{
			FLAG = 0x00;
			break;
		}
		dwNumber -= 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)szDATA);

		// 加自身编辑框的值
		SendMessage(hEdit_2, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber += 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_2, WM_SETTEXT, 0, (LPARAM)szDATA);

		// 释放互斥体标志
		::ReleaseMutex(g_hMutex);

		// 延时50毫秒
		Sleep(50);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_2(LPVOID lpParameter)
{
	// 第二个线程

	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS,FALSE, "g_hWIN32API_THREAD_FLAG");
	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};
	HWND hEdit_1 = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	HWND hEdit_3 = GetDlgItem((HWND)lpParameter, IDC_EDIT_3);
	do
	{
		::WaitForSingleObject(g_hMutex,INFINITE);
		SendMessage(hEdit_1, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (dwNumber < 50)
		{
			FLAG = 0x00;
			break;
		}
		dwNumber -= 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)szDATA);
		SendMessage(hEdit_3, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber += 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_3, WM_SETTEXT, 0, (LPARAM)szDATA);
		::ReleaseMutex(g_hMutex);
		Sleep(50);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_3(LPVOID lpParameter)
{
	// 第三个线程

	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, "g_hWIN32API_THREAD_FLAG");
	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};
	HWND hEdit_1 = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	HWND hEdit_4 = GetDlgItem((HWND)lpParameter, IDC_EDIT_4);
	do
	{
		::WaitForSingleObject(g_hMutex,INFINITE);
		SendMessage(hEdit_1, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (dwNumber < 50)
		{
			FLAG = 0x00;
			break;
		}
		dwNumber -= 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)szDATA);
		SendMessage(hEdit_4, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber += 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_4, WM_SETTEXT, 0, (LPARAM)szDATA);
		::ReleaseMutex(g_hMutex);
		Sleep(50);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// 用于创建3个线程并且监听他们

	// 创建互斥体标志
	HANDLE g_hMutex = ::CreateMutex(NULL, FALSE, "g_hWIN32API_THREAD_FLAG");
	if (!g_hMutex)
	{
		return RETURN_FAILED;
	}

	// 用于存储线程内核对象句柄
	HANDLE hThreadArrays[0x3] = {0};

	// 赋值
	hThreadArrays[0] = ::CreateThread(NULL, 0, ThreadProc_1, lpParameter, 0, NULL);
	hThreadArrays[1] = ::CreateThread(NULL, 0, ThreadProc_2, lpParameter, 0, NULL);
	hThreadArrays[2] = ::CreateThread(NULL, 0, ThreadProc_3, lpParameter, 0, NULL);
	
	// 等待全部线程执行完毕
	::WaitForMultipleObjects(3, hThreadArrays, TRUE, INFINITE);

	// 释放内核对象空间
	::CloseHandle(hThreadArrays[0]);
	::CloseHandle(hThreadArrays[1]);
	::CloseHandle(hThreadArrays[2]);
	::CloseHandle(g_hMutex);

	return RETURN_SUCCESS;
}
*/

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_1(LPVOID lpParameter)
{
	// 第三个线程

	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};
	HWND hEdit_1 = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	HWND hEdit_2 = GetDlgItem((HWND)lpParameter, IDC_EDIT_2);
	do
	{
		EnterCriticalSection(&csCriticalSection);
		SendMessage(hEdit_1, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (dwNumber < 50)
		{
			FLAG = 0x00;
			break;
		}
		dwNumber -= 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)szDATA);
		SendMessage(hEdit_2, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber += 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_2, WM_SETTEXT, 0, (LPARAM)szDATA);
		LeaveCriticalSection(&csCriticalSection);
		Sleep(50);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_2(LPVOID lpParameter)
{
	// 第三个线程

	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};
	HWND hEdit_1 = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	HWND hEdit_3 = GetDlgItem((HWND)lpParameter, IDC_EDIT_3);
	do
	{
		EnterCriticalSection(&csCriticalSection);
		SendMessage(hEdit_1, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (dwNumber < 50)
		{
			FLAG = 0x00;
			break;
		}
		dwNumber -= 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)szDATA);
		SendMessage(hEdit_3, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber += 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_3, WM_SETTEXT, 0, (LPARAM)szDATA);
		LeaveCriticalSection(&csCriticalSection);
		Sleep(50);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_3(LPVOID lpParameter)
{
	// 第三个线程

	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};
	HWND hEdit_1 = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	HWND hEdit_4 = GetDlgItem((HWND)lpParameter, IDC_EDIT_4);
	do
	{
		EnterCriticalSection(&csCriticalSection);
		SendMessage(hEdit_1, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (dwNumber < 50)
		{
			FLAG = 0x00;
			break;
		}
		dwNumber -= 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_1, WM_SETTEXT, 0, (LPARAM)szDATA);
		SendMessage(hEdit_4, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber += 50;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit_4, WM_SETTEXT, 0, (LPARAM)szDATA);
		LeaveCriticalSection(&csCriticalSection);
		Sleep(50);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// 用于创建3个线程并且监听他们

	// 初始化令牌
	InitializeCriticalSection(&csCriticalSection);

	// 用于存储线程内核对象句柄
	HANDLE hThreadArrays[0x3] = {0};

	// 赋值
	hThreadArrays[0] = ::CreateThread(NULL, 0, ThreadProc_1, lpParameter, 0, NULL);
	hThreadArrays[1] = ::CreateThread(NULL, 0, ThreadProc_2, lpParameter, 0, NULL);
	hThreadArrays[2] = ::CreateThread(NULL, 0, ThreadProc_3, lpParameter, 0, NULL);
	
	// 等待全部线程执行完毕
	::WaitForMultipleObjects(3, hThreadArrays, TRUE, INFINITE);
	
	// 释放内核对象空间
	::CloseHandle(hThreadArrays[0]);
	::CloseHandle(hThreadArrays[1]);
	::CloseHandle(hThreadArrays[2]);

	// 卸载令牌
	DeleteCriticalSection(&csCriticalSection);

	// 更改全局标志
	FLAG = 0x00;

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
	/*
		用于处理"IDD_DIALOG_MAIN"对话框的回调函数
	*/
	
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
			// 界面初始化
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)"1000");
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_3), WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_4), WM_SETTEXT, 0, (LPARAM)"0");
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_1:
				{
					// 启动任务线程
					if (!FLAG)
					{
						FLAG = 0x01;
						HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc, hwndDlg, 0, NULL);
						::CloseHandle(hThread);
						break;
					}
					else
					{
						break;
					}
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
 	// TODO: Place code here.
	/*
		抢红包练习：
		第一步：在第一个文本框中输入一个值，比如1000
		第二步：点击抢红包，同时创建3个线程，每个线程循环进行抢红包的操作，每次抢50
		第三步：使用Mutex进行线程控制，当第一个文本框中的值<50时，将红包线程结束.
		特别说明：
		1、四个文本框中的值总和应该为1000
		2、抢红包线程每次延时50毫秒.
		3、使用WaitForMultipleObjects监听所有线程，当线程全部结束后
		   调用CloseHandle关闭句柄.
	*/

	// 利用内核对象防止多开
	HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = CheckIsExsist();

	// 调用对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);

	// 当程序将要结束前释放内核对象空间
	::CloseHandle(g_hCHECK_EXSIST_WIN32API_FLAG);
	
	return 0;
}
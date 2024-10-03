// Win32Api_14.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

// 信号量标志
HANDLE g_hSemaphore = {0};
HANDLE g_hEnd = {0};
// 事件标志
HANDLE g_hTop = {0};
HANDLE g_hUnder = {0};
// 线程结束标志
BYTE g_bFLAG = 0x00;
// 存储线程对应的窗口句柄
HWND g_hThread[0x7] = {0};

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcTop(LPVOID lpParameter)
{
	// TOP线程处理函数
	// 用于存放获取到的字符串和整数
	TCHAR szDATA[0x20] = {0};
	TCHAR szNum[0x10] = {0};
	// 用于单个需要设置的字符串,带结束符
	TCHAR szSetText[0x2] = {0};
	// 获取文本框的内容并放置到szDATA中
	SendMessage(g_hThread[0], WM_GETTEXT, 0x10, (LPARAM)szDATA);
	// 获取字符串个数用于循环
	DWORD dwCount = strlen(szDATA);
	// 字符串数组指针
	DWORD dwPStr = 0;
	do
	{
		// 等待TOP事件标志切换为已通知状态
		::WaitForSingleObject(g_hTop, INFINITE);
		Sleep(1000);
		// 将值拆分放置到单个字符文本框
		memcpy(szSetText, szDATA + dwPStr++, 0x1);
		// 获取缓冲区文本框内容,看谁的值是0
		SendMessage(g_hThread[1], WM_GETTEXT, 0x10, (LPARAM)szNum);
		if (!strcmp(szNum, "0"))
		{
			// 设置第一个缓冲区
			SendMessage(g_hThread[1], WM_SETTEXT, 0, (LPARAM)szSetText);
		}
		else
		{
			// 设置第二个缓冲区
			SendMessage(g_hThread[2], WM_SETTEXT, 0, (LPARAM)szSetText);
		}
		// 更改g_hUnder标志,让其它线程继续运行
		::SetEvent(g_hUnder);
	}
	while(--dwCount);
	// 设置全局标志
	g_bFLAG = 0x01;
	// 等待信号量标志切换为已通知状态
	::WaitForSingleObject(g_hEnd, INFINITE);
	// 结束标志
	g_bFLAG = 0x02;
	return 0xFF;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcUnder(LPVOID lpParameter)
{
	// UNDER子线程处理函数
	// 转换下标
	DWORD dwIndex = (DWORD)lpParameter;
	// 用于存放获取到的字符串和整数
	TCHAR szDATA[0x10] = {0};
	// 拼接的字符串
	TCHAR szString[0x10] = {0};
	do
	{
		// 等待信号量标志切换为已通知状态
		::WaitForSingleObject(g_hSemaphore, INFINITE);
		// 等待UNDER事件标志切换为已通知状态
		::WaitForSingleObject(g_hUnder, INFINITE);
		Sleep(1000);
		// 判断线程结束标志
		if (g_bFLAG == 0x02)
		{
			::ReleaseSemaphore(g_hSemaphore, 1, NULL);
			::SetEvent(g_hUnder);
			break;
		}
		// 获取缓冲区1的值及自身的值
		SendMessage(g_hThread[1], WM_GETTEXT, 0x10, (LPARAM)szDATA);
		SendMessage(g_hThread[dwIndex], WM_GETTEXT, 0x10, (LPARAM)szString);
		// 将有值的缓冲区的值放进自己的编辑框,并清0缓冲区
		if (strcmp(szDATA, "0"))
		{
			// 有值则将第1个缓冲区的值,加上原本的值,设置到自己的编辑框中
			strcat(szString, szDATA);
			strcat(szString, "-");
			SendMessage(g_hThread[dwIndex], WM_SETTEXT, 0, (LPARAM)szString);
			// 清0缓冲区
			SendMessage(g_hThread[1], WM_SETTEXT, 0, (LPARAM)"0");
		}
		else
		{
			// 没有值则将第2个缓冲区的值读出来,加上原本的值,设置到自己的编辑框中
			SendMessage(g_hThread[2], WM_GETTEXT, 0x10, (LPARAM)szDATA);
			strcat(szString, szDATA);
			strcat(szString, "-");
			SendMessage(g_hThread[dwIndex], WM_SETTEXT, 0, (LPARAM)szString);
			// 清0缓冲区
			SendMessage(g_hThread[2], WM_SETTEXT, 0, (LPARAM)"0");
		}

		// 释放1个信号
		::ReleaseSemaphore(g_hSemaphore, 1, NULL);
		// 更改g_hTop标志,让另一个线程继续运行
		::SetEvent(g_hTop);
		if (g_bFLAG== 0x01)
		{
			::ReleaseSemaphore(g_hEnd, 1, NULL);
			::SetEvent(g_hUnder);
		}
	}
	while(TRUE);

	return dwIndex;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// 主任务线程
	// 创建信号量内核对象
	g_hSemaphore = ::CreateSemaphore(NULL, 0, 4, NULL);
	g_hEnd = ::CreateSemaphore(NULL, 0, 1, NULL);
	// 创建事件内核对象
	g_hTop = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hUnder = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	// 创建存储线程内核对象句柄的数组
	HANDLE hThreadArrays[0x5] = {0};
	// 创建1个TOP线程
	hThreadArrays[0] = ::CreateThread(NULL, 0, ThreadProcTop, NULL, 0, NULL);
	// 创建4个UNDER线程
	hThreadArrays[1] = ::CreateThread(NULL, 0, ThreadProcUnder, (LPVOID)3, 0, NULL);
	hThreadArrays[2] = ::CreateThread(NULL, 0, ThreadProcUnder, (LPVOID)4, 0, NULL);
	hThreadArrays[3] = ::CreateThread(NULL, 0, ThreadProcUnder, (LPVOID)5, 0, NULL);
	hThreadArrays[4] = ::CreateThread(NULL, 0, ThreadProcUnder, (LPVOID)6, 0, NULL);
	// 释放信号,启动UNDER线程
	ReleaseSemaphore(g_hSemaphore, 2, NULL);
	// 设置事件标志,启动TOP线程
	::SetEvent(g_hTop);
	// 等待全部线程执行完毕
	::WaitForMultipleObjects(5, hThreadArrays, TRUE, INFINITE);
	// 置零标志位
	g_bFLAG = 0x00;
	// 释放线程内核对象空间
	::CloseHandle(hThreadArrays[0]);
	::CloseHandle(hThreadArrays[1]);
	::CloseHandle(hThreadArrays[2]);
	::CloseHandle(hThreadArrays[3]);
	::CloseHandle(hThreadArrays[4]);
	// 释放信号量内核对象
	::CloseHandle(g_hSemaphore);
	::CloseHandle(g_hEnd);
	// 释放事件内核对象
	::CloseHandle(g_hTop);
	::CloseHandle(g_hUnder);
	return 0xEE;
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
			// 存储窗口句柄到全局变量中
			g_hThread[0] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_1);
			g_hThread[1] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_2);
			g_hThread[2] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_3);
			g_hThread[3] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_4);
			g_hThread[4] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_5);
			g_hThread[5] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_6);
			g_hThread[6] = GetDlgItem((HWND)hwndDlg, IDC_EDIT_7);
			// 界面初始化
			SendMessage(g_hThread[0], WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(g_hThread[1], WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(g_hThread[2], WM_SETTEXT, 0, (LPARAM)"0");
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
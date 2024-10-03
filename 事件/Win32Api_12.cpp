// Win32Api_12.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

// 事件对象
HANDLE g_hSet = 0;
HANDLE g_hClear = 0;

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_1(LPVOID lpParameter)
{
	// 第一个线程(每次循环将文本框中的值减2并写回,需要和线程2交替执行)

	// 存储需要转换的整形和字符串
	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};

	// 转型
	HWND hEdit = (HWND)lpParameter;

	do
	{
		// 监听g_hSet是否是已通知状态(这里WaitForSingleObject执行完毕后会自动改为未通知状态)
		::WaitForSingleObject(g_hSet, INFINITE);

		// 减目标编辑框中的值
		SendMessage(hEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (!dwNumber)
		{
			break;
		}
		dwNumber -= 2;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)szDATA);

		// 延时1秒
		Sleep(1000);

		// 更改g_hClear标志,让另一个线程继续运行
		::SetEvent(g_hClear);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_2(LPVOID lpParameter)
{
	// 第二个线程(每次循环将文本框中的值减3并写回,需要和线程1交替执行)

	// 存储需要转换的整形和字符串
	DWORD dwNumber = 0;
	TCHAR szDATA[0x10] = {0};

	// 转型
	HWND hEdit = (HWND)lpParameter;

	do
	{
		// 监听g_hSet是否是已通知状态(这里WaitForSingleObject执行完毕后会自动改为未通知状态)
		::WaitForSingleObject(g_hClear, INFINITE);

		// 减目标编辑框中的值
		SendMessage(hEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		if (!dwNumber)
		{
			break;
		}
		dwNumber -= 3;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)szDATA);

		// 延时1秒
		Sleep(1000);

		// 更改g_hClear标志,让另一个线程继续运行
		::SetEvent(g_hSet);
	}
	while(TRUE);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// 用于创建2个线程并且监听他们

	// 得到文本框的句柄
	HWND hEdit = GetDlgItem((HWND)lpParameter, IDC_EDIT_1);

	// 用于存储线程内核对象句柄
	HANDLE hThreadArrays[0x2] = {0};

	// 创建事件内核对象并赋值
	// 第二个参数表示:每次wait到后,要修改状态为未通知
	// 第三个参数表示:是否事件一被创建出来就会被wait到
	g_hSet = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hClear = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	// 给内核对象数组赋值
	hThreadArrays[0] = ::CreateThread(NULL, 0, ThreadProc_1, hEdit, 0, NULL);
	hThreadArrays[1] = ::CreateThread(NULL, 0, ThreadProc_2, hEdit, 0, NULL);

	// 主线程先给文本框赋值(此时其它两个线程阻塞)
	SendMessage((HWND)hEdit, WM_SETTEXT, 0, (LPARAM)"20");

	// 延迟2秒用于更好的显示阻塞状态
	Sleep(2000);

	// 将要先执行的线程设置为已通知状态
	::SetEvent(g_hSet);

	// 等待全部线程执行完毕
	::WaitForMultipleObjects(2, hThreadArrays, TRUE, INFINITE);

	// 释放内核对象空间
	::CloseHandle(hThreadArrays[0]);
	::CloseHandle(hThreadArrays[1]);

	// 销毁事件内核对象
    ::CloseHandle(g_hSet);  		
    ::CloseHandle(g_hClear);  		

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
			// 界面初始化(先不在这里初始化文本框,用于测试线程)
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)"0");
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_1:
				{
					// 启动任务线程
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
 	// TODO: Place code here.

	// 利用互斥体防止多开
	HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = CheckIsExsist();

	// 调用对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);

	// 当程序将要结束前关闭内核句柄
	::CloseHandle(g_hCHECK_EXSIST_WIN32API_FLAG);

	return 0;
}
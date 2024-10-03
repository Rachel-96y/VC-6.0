// Win32Api_10.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

BYTE Thread1 = 0x00;
BYTE Thread2 = 0x00;
BYTE Thread3 = 0x00;

// 令牌结构需要是全局的
CRITICAL_SECTION csCriticalSection = {0};
CRITICAL_SECTION csCriticalSection_2 = {0};

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc1(LPVOID hwndDlg)
{
	if (Thread1)
	{
		return 0;
	}
	// 更改标志
	Thread1 = 0x01;
	// 创建一个缓冲区用于存储获取到的文本字符串
	TCHAR szDATA[0x10] = {0};
	// 用于存储转换后的整数
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_1);
	do
	{
		Sleep(3);
		// 获取文本框中的值
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		// 将字符串转换为整形
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		// 将整形转换为字符串
		sprintf(szDATA, "%d", dwNumber);
		// 重新给文本框赋值
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
	}
	while(--dwCount);

	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc2(LPVOID hwndDlg)
{
	if (Thread2)
	{
		return 0;
	}
	Thread2 = 0x01;
	TCHAR szDATA[0x10] = {0};
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_1);
	do
	{
		Sleep(3);
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
	}
	while(--dwCount);

	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc3(LPVOID hwndDlg)
{
	if (Thread3)
	{
		return 0;
	}
	Thread3 = 0x01;
	TCHAR szDATA[0x10] = {0};
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_1);
	do
	{
		Sleep(3);
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
	}
	while(--dwCount);

	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcNew1(LPVOID hwndDlg)
{
	if (Thread1)
	{
		return 0;
	}
	// 更改标志
	Thread1 = 0x01;
	// 创建一个缓冲区用于存储获取到的文本字符串
	TCHAR szDATA[0x10] = {0};
	// 用于存储转换后的整数
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_1);
	do
	{
		Sleep(3);
		// 获取令牌
		EnterCriticalSection(&csCriticalSection);
		// 获取文本框中的值
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		// 将字符串转换为整形
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		// 将整形转换为字符串
		sprintf(szDATA, "%d", dwNumber);
		// 重新给文本框赋值
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
		// 归还令牌
		LeaveCriticalSection(&csCriticalSection);
	}
	while(--dwCount);

	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcNew2(LPVOID hwndDlg)
{
	if (Thread2)
	{
		return 0;
	}
	// 更改标志
	Thread2 = 0x01;
	TCHAR szDATA[0x10] = {0};
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_1);
	do
	{
		Sleep(3);
		EnterCriticalSection(&csCriticalSection);
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
		LeaveCriticalSection(&csCriticalSection);
	}
	while(--dwCount);

	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcNew3(LPVOID hwndDlg)
{
	if (Thread3)
	{
		return 0;
	}
	Thread3 = 0x01;
	TCHAR szDATA[0x10] = {0};
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_1);
	do
	{
		Sleep(3);
		EnterCriticalSection(&csCriticalSection);
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
		LeaveCriticalSection(&csCriticalSection);
	}
	while(--dwCount);

	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcErro1(LPVOID hwndDlg)
{
	if (Thread1)
	{
		return 0;
	}
	// 更改标志
	Thread1 = 0x01;
	EnterCriticalSection(&csCriticalSection_2);
	// 创建一个缓冲区用于存储获取到的文本字符串
	TCHAR szDATA[0x10] = {0};
	// 用于存储转换后的整数
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_2);
	do
	{
		// 获取令牌
		EnterCriticalSection(&csCriticalSection);
		// 获取文本框中的值
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		// 将字符串转换为整形
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		// 将整形转换为字符串
		sprintf(szDATA, "%d", dwNumber);
		// 重新给文本框赋值
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
		// 归还令牌
		LeaveCriticalSection(&csCriticalSection);
	}
	while(--dwCount);

	LeaveCriticalSection(&csCriticalSection_2);
	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcErro2(LPVOID hwndDlg)
{
	if (Thread2)
	{
		return 0;
	}
	// 更改标志
	Thread2 = 0x01;
	EnterCriticalSection(&csCriticalSection_2);
	TCHAR szDATA[0x10] = {0};
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_2);
	do
	{
		EnterCriticalSection(&csCriticalSection);
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		sprintf(szDATA, "%d", dwNumber);
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
		LeaveCriticalSection(&csCriticalSection);
	}
	while(--dwCount);

	LeaveCriticalSection(&csCriticalSection_2);
	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcErro3(LPVOID hwndDlg)
{
	if (Thread3)
	{
		return 0;
	}
	// 更改标志
	Thread3 = 0x01;
	TCHAR szDATA[0x10] = {0};
	DWORD dwNumber = 0;
	DWORD dwCount = 2000;
	HWND hwndEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_2);
	do
	{
		EnterCriticalSection(&csCriticalSection);
		SendMessage(hwndEdit, WM_GETTEXT, 0x10, (LPARAM)szDATA);
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		sprintf(szDATA, "%d", dwNumber);
		EnterCriticalSection(&csCriticalSection_2);
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szDATA);
		LeaveCriticalSection(&csCriticalSection_2);
		LeaveCriticalSection(&csCriticalSection);
	}
	while(--dwCount);

	
	return 0;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_EDIT(LPVOID hwndDlg)
{
	HWND hEdit = (HWND)hwndDlg;
	TCHAR szDATA[0x10] = {0};
	
	do
	{
	Sleep(1000);
	SendMessage(hEdit, WM_SETTEXT, 1, (LPARAM)"临界区结构体参数:\r\n");
	SendMessage(hEdit, EM_SETSEL, -2, -1);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"DebugInfo:			");
	sprintf(szDATA, "%d",csCriticalSection.DebugInfo);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"LockCount:			");
	sprintf(szDATA, "%d",csCriticalSection.LockCount);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"LockSemaphore:			");
	sprintf(szDATA, "%d",csCriticalSection.LockSemaphore);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"OwningThread:			");
	sprintf(szDATA, "%d",csCriticalSection.OwningThread);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"RecursionCount:			");
	sprintf(szDATA, "%d",csCriticalSection.RecursionCount);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"SpinCount:			");
	sprintf(szDATA, "%08X",csCriticalSection.SpinCount);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");
	}
	while(TRUE);

	
	return 0;
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
			// 默认选择一个单选框(复选框)
			CheckDlgButton(hwndDlg, IDC_RADIO_1, BST_CHECKED);

			// 设置第一个Edit
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)"0");

			// 设置第二个Edit
			SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)"0");

			// 获取第三个Edit的句柄
			HWND hEdit = GetDlgItem((HWND)hwndDlg, IDC_EDIT_Info);

			// 启动第一个线程实时打印结构体的值
			::CreateThread(NULL, 0, ThreadProc_EDIT, hEdit, 0, NULL);

			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_RUN_1:
				{
					// 启动线程1
					DWORD dwFlag_1 = IsDlgButtonChecked(hwndDlg, IDC_RADIO_1);
					if (dwFlag_1 == BST_CHECKED)
					{
						::CreateThread(NULL, 0, ThreadProcNew1, hwndDlg, 0, NULL);
					}
					else
					{
						::CreateThread(NULL, 0, ThreadProc1, hwndDlg, 0, NULL);
					}
					break;
				}
			case IDC_BUTTON_RUN_2:
				{
					// 启动线程2
					DWORD dwFlag_2 = IsDlgButtonChecked(hwndDlg, IDC_RADIO_1);
					if (dwFlag_2 == BST_CHECKED)
					{
						::CreateThread(NULL, 0, ThreadProcNew2, hwndDlg, 0, NULL);
					}
					else
					{
						::CreateThread(NULL, 0, ThreadProc2, hwndDlg, 0, NULL);
					}
					break;
				}
			case IDC_BUTTON_RUN_3:
				{
					// 启动线程3
					// 判断单选框是否被勾选(复选框也是此方法)
					DWORD dwFlag_3 = IsDlgButtonChecked(hwndDlg, IDC_RADIO_1);
					if (dwFlag_3 == BST_CHECKED)
					{
						::CreateThread(NULL, 0, ThreadProcNew3, hwndDlg, 0, NULL);
					}
					else
					{
						::CreateThread(NULL, 0, ThreadProc3, hwndDlg, 0, NULL);
					}
					break;
				}
			case IDC_BUTTON_RUN_4:
				{
					// 同时启动3个线程实现死锁
					// 判断单选框是否被勾选(复选框也是此方法)
					::CreateThread(NULL, 0, ThreadProcErro1, hwndDlg, 0, NULL);
					::CreateThread(NULL, 0, ThreadProcErro2, hwndDlg, 0, NULL);
					::CreateThread(NULL, 0, ThreadProcErro3, hwndDlg, 0, NULL);
					break;
				}
			case IDC_BUTTON_RUN_Clear:
				{
					// 置零值
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)"0");
					SendMessage(GetDlgItem((HWND)hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)"0");
					Thread1 = 0x00;
					Thread2 = 0x00;
					Thread3 = 0x00;
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
	/*
		本节练习：	
		通过使用临界区实现一个死锁程序.			
	*/

 	// 首先检测是否此程序只运行了一个实例
	CheckIsExsist();

	// 初始化令牌
	InitializeCriticalSection(&csCriticalSection);
	InitializeCriticalSection(&csCriticalSection_2);

	// 调用对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);

	// 卸载令牌
	DeleteCriticalSection(&csCriticalSection);
	DeleteCriticalSection(&csCriticalSection_2);

	return 0;
}
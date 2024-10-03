// Win32Api_8.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define CALLBACK_COMPLETED TRUE
#define CALLBACK_UNCOMPLETED FALSE

// 用于判断是否函数还未执行完成
BYTE FLAG = 0;

DWORD WINAPI ThreadProcADD(LPVOID hwndEdit)
{

	FLAG = 1;
	// 创建一个缓冲区用于存储获取到的文本字符串
	TCHAR szDATA[0x10] = {0};
	// 用于存储转换后的整数
	DWORD dwNumber = 0;
	DWORD dwCount = 15;
	do
	{
		// 获取文本框中的值
		SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT_ADD), WM_GETTEXT, 0x10, (LPARAM)szDATA);
		// 将字符串转换为整形
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber--;
		// 每过1秒减一次
		Sleep(1000);
		// 将整形转换为字符串
		sprintf(szDATA, "%d", dwNumber);
		// 重新给文本框赋值
		SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT_ADD), WM_SETTEXT, 0, (LPARAM)szDATA);
	}
	while(--dwCount);

	FLAG = 0;

	return 0;
}

DWORD WINAPI ThreadProcSUB(LPVOID hwndEdit)
{
	// 创建一个缓冲区用于存储获取到的文本字符串
	TCHAR szDATA[0x10] = {0};
	// 用于存储转换后的整数
	DWORD dwNumber = 0;
	DWORD dwCount = 15;
	do
	{
		// 获取文本框中的值
		SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT_SUB), WM_GETTEXT, 0x10, (LPARAM)szDATA);
		// 将字符串转换为整形
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		// 每过1秒减一次
		Sleep(1000);
		// 将整形转换为字符串
		sprintf(szDATA, "%d", dwNumber);
		// 重新给文本框赋值
		SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT_SUB), WM_SETTEXT, 0, (LPARAM)szDATA);
	}
	while(--dwCount);

	return 0;
}

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
			SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_ADD), WM_SETTEXT, 0, (LPARAM)"15");
			SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_SUB), WM_SETTEXT, 0, (LPARAM)"0");
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_RUN:
				{
				if (!FLAG)
					{
						SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_ADD), WM_SETTEXT, 0, (LPARAM)"15");
						SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_SUB), WM_SETTEXT, 0, (LPARAM)"0");
						FLAG = 1;
						HANDLE hThreadADD = ::CreateThread(NULL, 0, ThreadProcADD, hwndDlg, 0, NULL);
						HANDLE hThreadSUB = ::CreateThread(NULL, 0, ThreadProcSUB, hwndDlg, 0, NULL);
						::CloseHandle(hThreadADD);
						::CloseHandle(hThreadSUB);
					}
				break;
				}
			case IDC_BUTTON_TEST:
				{
					if (FLAG == 1)
					{
						DWORD dwResult = MessageBox(hwndDlg, "界面卡顿吗?", "界面测试", MB_YESNOCANCEL | MB_ICONQUESTION);
						if (dwResult == IDYES)
						{
							MessageBox(hwndDlg, "很不幸", "界面测试", MB_ICONWARNING);
						}
						else if(dwResult == IDNO)
						{
							MessageBox(hwndDlg, "太棒了!", "成功!", MB_ICONINFORMATION | MB_ICONASTERISK);
						}
					}
					else if (!FLAG)
					{
						MessageBox(hwndDlg, "你还没有点击开始!", "界面测试", MB_ICONWARNING);
					}
					break;
				}
			case IDC_BUTTON_STOP:
				{
					if (FLAG)
					{
						MessageBox(hwndDlg, "我不知道怎么停止他┭┮﹏┭┮!", "界面测试", MB_SYSTEMMODAL);
					}
					else
					{
						MessageBox(hwndDlg, "你还没有点击开始!", "界面测试", MB_ICONWARNING);
					}
					break;
				}

			} // End switch
		return CALLBACK_COMPLETED;
		} 
	}// End switch
	return CALLBACK_UNCOMPLETED;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	/*
		本节练习：
			要求：
			点击开始后，第一个文本框的值从1000每隔1秒种减1 直到0
			第二个文本框从0每隔1秒钟加1 直到1000
	*/

	// 检查是否只有一个同名程序
	CheckIsExsist();

	// 调用对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);

	return 0; 
}
// Win32Api_9.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define CALLBACK_COMPLETED			TRUE
#define CALLBACK_UNCOMPLETED		FALSE

// 开始
BYTE FLAG = 0x00;

// 终止
BYTE FLAG_2 = 0x00;  // 方式1
BYTE FLAG_4 = 0x00;  // 方式2

// 挂起
BYTE FLAG_3 = 0x00;

// 打印
BYTE FLAG_5 = 0x00;

HANDLE hThread = NULL;

// 用于传入多个参数
typedef struct _Args{
	HWND	hEdit;
	LPVOID	lpThread;
} Args, *PArgs;

Args pStArgs = {0};

DWORD WINAPI ThreadProcButton_1(LPVOID hwndEdit)
{
	FLAG = 0x01;

	// 创建一个缓冲区用于存储获取到的文本字符串
	TCHAR szDATA[0x10] = {0};
	// 用于存储转换后的整数
	DWORD dwNumber = 0;
	DWORD dwCount = 1000;
	do
	{
		if (FLAG_2 == 0x01)
		{
			break;
		}
		if (FLAG_4 == 0x01)
		{
			FLAG_4 = 0x00;
			FLAG = 0x00;
			FLAG_2 = 0x00;
			SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)"0");
			::ExitThread(222);
		}
		// 获取文本框中的值
		SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT), WM_GETTEXT, 0x10, (LPARAM)szDATA);
		// 将字符串转换为整形
		sscanf(szDATA, "%d", &dwNumber);
		dwNumber++;
		// 每过1秒减一次
		Sleep(100);
		// 将整形转换为字符串
		sprintf(szDATA, "%d", dwNumber);
		// 重新给文本框赋值
		SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)szDATA);
	}
	while(--dwCount);

	FLAG = 0x00;
	FLAG_2 = 0x00;
	SendMessage(GetDlgItem((HWND)hwndEdit, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)"0");

	return 111;
}

DWORD WINAPI ThreadProcOfEdit(LPVOID pArgs)
{
	FLAG_5 = 0x01;

	// 获取挂起的线程的寄存器的值
	DWORD dwExitCode = 0;
	DWORD dwResult = GetExitCodeThread(((PArgs)pArgs) -> lpThread, &dwExitCode);
	if (!dwResult || dwExitCode != STILL_ACTIVE)
	{
		MessageBox(((PArgs)pArgs) -> hEdit, "线程还没有执行", "错误的操作", MB_ICONASTERISK);
		return 444;
	}
	if (!FLAG_3)
	{
		MessageBox(((PArgs)pArgs) -> hEdit, "你需要先挂起线程才能准确获取当前值", "错误的操作", MB_ICONASTERISK);
		return 444;
	}

	// 创建context结构
	CONTEXT context;
	memset(&context, 0x00000000, sizeof(CONTEXT));

	// 设置要获取的类型				
	context.ContextFlags = CONTEXT_DEBUG_REGISTERS | CONTEXT_FULL;

	// 获取
	BOOL ret = ::GetThreadContext(((PArgs)pArgs) -> lpThread, &context);
	if (!ret)
	{
		MessageBox(((PArgs)pArgs) -> hEdit, "获取寄存器值失败!", "失败了", MB_ICONASTERISK);
		return 444;
	}

	// 获取文本框句柄
	HWND hwndEdit = GetDlgItem(((PArgs)pArgs) -> hEdit, IDC_EDIT_Register);

	// 用于存储转换后的字符串
	TCHAR szDATA[0x10] = {0};

	// 寄存器名称
	PTCHAR szName[0x16] = 
	{
		"Dr0",
		"Dr1",
		"Dr2",
		"Dr3",
		"Dr6",
		"Dr7",
		"SegGs",
		"SegFs",
		"SegEs",
		"SegDs",
		"Edi",
		"Esi",
		"Ebx",
		"Edx",
		"Ecx",
		"Eax",
		"Ebp",
		"Eip",
		"SegCs",
		"EFlags",
		"Esp",
		"SegSs",
	};

	// 寄存器对应的值
	DWORD dwDATA[0x16] = 
	{
		context.Dr0,
		context.Dr1,
		context.Dr2,
		context.Dr3,
		context.Dr6,
		context.Dr7,
		context.SegGs,
		context.SegFs,
		context.SegEs,
		context.SegDs,
		context.Edi,
		context.Esi,
		context.Ebx,
		context.Edx,
		context.Ecx,
		context.Eax,
		context.Ebp,
		context.Eip,
		context.SegCs,
		context.EFlags,
		context.Esp,
		context.SegSs,
	};

	// 标题
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"当前寄存器的值: \r\n\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);
	
	// 用于记数
	DWORD dwCount = 0;
	do
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szName[dwCount]);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"				");
		sprintf(szDATA, "%08X", dwDATA[dwCount]);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
	}
	while(++dwCount < 0x16);
	// 设置
	// context.Eip = 0x401000;
	// SetThreadContext(hThread,&context);

	return 444;
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
			SendMessage(GetDlgItem(hwndDlg, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)"0");
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_1:
				{
					// 开始
					if (!FLAG)
					{
						SendMessage(GetDlgItem(hwndDlg, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)"0");
						hThread = ::CreateThread(NULL, 0, ThreadProcButton_1, hwndDlg, 0, NULL);
					}
					break;
				}
			case IDC_BUTTON_2:
				{
					// 挂起
					if (!FLAG_3)
					{
						FLAG_3 = 0x01;
						FLAG_5 = 0x00;
						::SuspendThread(hThread);
					}
					break;
				}
			case IDC_BUTTON_3:
				{
					// 继续
					if (FLAG_3)
					{
						FLAG_3 = 0x00;
						::ResumeThread(hThread);
					}
					break;
				}
			case IDC_BUTTON_4:
				{
					// 终止方式1
					DWORD dwExitCode = 0;
					GetExitCodeThread(hThread, &dwExitCode);

					if (!FLAG_2 && dwExitCode == STILL_ACTIVE)
					{
						FLAG_2 = 0x01;
					}
					
					break;
				}
			case IDC_BUTTON_5:
				{
					// 终止方式2
					DWORD dwExitCode = 0;
					GetExitCodeThread(hThread, &dwExitCode);
					if (!FLAG_4 && dwExitCode == STILL_ACTIVE)
					{
						FLAG_4 = 0x01;
					}
					break;
				}
			case IDC_BUTTON_6:
				{
					// 终止方式3
					::TerminateThread(hThread, 333);
					::WaitForSingleObject(hThread, INFINITE);
					FLAG = 0x00;
					FLAG_2 = 0x00;
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)"0");
					break;
				}
			case IDC_BUTTON_7:
				{
					// 查看退出码
					DWORD dwExitCode = 0;
					DWORD dwResult = GetExitCodeThread(hThread, &dwExitCode);
					if (!dwResult)
					{
						MessageBox(hwndDlg, "没有任何退出码!", 0, MB_ICONWARNING);
						break;
					}
					if (dwExitCode == STILL_ACTIVE)
					{
						MessageBox(hwndDlg, "程序还在运行中!", 0, MB_ICONWARNING);
						break;
					}
					TCHAR szDATA[0x10] = {0};
					sprintf(szDATA, "%d", dwExitCode);
					MessageBox(hwndDlg, szDATA, "显示退出码", MB_ICONASTERISK);
					break;
				}
			case IDC_BUTTON_8:
				{
					if (!FLAG_5)
					{
						pStArgs.hEdit = hwndDlg;
						pStArgs.lpThread = hThread;
						::CreateThread(NULL, 0, ThreadProcOfEdit, &pStArgs, 0, NULL);
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
 	// 首先检测是否此程序只运行了一个实例
	CheckIsExsist();

	// 调用对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);

	return 0;
}
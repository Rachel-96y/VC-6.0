// Win32Api_5.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

/*
	DWORD Erro = ::GetLastError();
	DbgPrintf("%d\n", Erro);
*/

HINSTANCE hAppInstance = NULL;

BOOL CALLBACK DialogProc(
						 HWND hwndDlg,  // handle to dialog box
						 UINT uMsg,     // message			
						 WPARAM wParam, // first message parameter
						 LPARAM lParam  // second message parameter
						 )
{
	/*
		资源文件_消息断点
	*/
	
	switch(uMsg)								
	{
	case  WM_INITDIALOG:								
		{		
			//设置图标
			HICON hIcon_SMALL = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));
			HICON hIcon_BIG = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_BIG));
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon_SMALL);
			SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon_BIG);

			return TRUE;							
		}			
	case  WM_COMMAND:
		{
			switch (LOWORD(wParam))							
			{
			case IDC_BUTTON_1:
				{
					// 获取文本框的句柄
					HWND hEdit = GetDlgItem(hwndDlg, IDC_EDIT);
					// 得到内容
					TCHAR szUserBuff[0x20];
					TCHAR LPassWord[0x20] = "18654aefef4864g6reg13e";
					GetWindowText(hEdit, szUserBuff, 0x20);
					DbgPrintf("%s\n", szUserBuff);
					int Ret = strcmp(szUserBuff, LPassWord);
					if (!Ret)
					{
						MessageBox(NULL,TEXT("密码正确!"),TEXT("Check"),MB_OK);
					}
					else
					{
						MessageBox(NULL,TEXT("密码错误!"),TEXT("Check"),MB_OK);
					}
					
					return TRUE;
				}
			case IDC_BUTTON_2:
				{			
					EndDialog(hwndDlg, 0);						
											
					return TRUE;
				}
			case WM_DESTROY:
				{
					// 点击关闭按钮
					EndDialog(hwndDlg, 0);
					return TRUE;
				}

			}
		}					
    }														
	
	return FALSE;							
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	
	// 利用互斥体防止多开
	HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = CheckIsExsist();

 	// TODO: Place code here.
	hAppInstance = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1_MAIN), NULL, DialogProc);

		// 当程序将要结束前关闭内核句柄
	::CloseHandle(g_hCHECK_EXSIST_WIN32API_FLAG);

	
	return 0;
}

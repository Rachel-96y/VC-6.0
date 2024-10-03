// WIN32API_7.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PeFiles.h"

//////////////////////////////////////////////////////////////////////
// 此文件是程序入口文件
// DWORD Erro = ::GetLastError();
// DbgPrintf("%d\n", Erro);
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 全局指针
TLSDirectory g_stTLS;

BOOL CALLBACK DialogProcDS_ALL(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	/*
		用于处理"IDD_DIALOG_DS_Export"对话框的回调函数
	*/

	switch(uMsg)
	{
	case  WM_INITDIALOG:
		{
			// 设置小图标及大图标为同一个图标
			SetIcon(g_stTLS.m_hAppInstance, hwndDlg, IDI_ICON_ALL, IDI_ICON_ALL);
			if (FILE_OF_PE32_COMPARE(g_stTLS.m_dwGlobalFlag))
			{
				// 根据标志位执行不同函数
				switch (CLICK_DS_COMPARE(g_stTLS.m_dwGlobalFlag))
				{
				case CLICK_DS_EXPORT:
					{
						// 32位导出表
						AnalysisDsExportOfPEData32(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_IMPORT:
					{
						// 32位导入表
						AnalysisDsImportOfPEData32(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_RESOURCE:
					{
						// 32位资源表
						AnalysisDsResourceOfPEData32(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_BASERELOC:
					{
						// 32位重定位表
						AnalysisDsBaseRelocOfPEData32(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_BOUNDIMPORT:
					{
						// 32位绑定导入表
						AnalysisDsBoundImportOfPEData32(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_IAT:
					{
						// 32位IAT表
						AnalysisDsIATOfPEData32(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				} // End switch
			
			}
			else
			{
				// 64位
				switch (CLICK_DS_COMPARE(g_stTLS.m_dwGlobalFlag))
				{
				case CLICK_DS_EXPORT:
					{
						// 64位导出表
						AnalysisDsExportOfPEData64(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_IMPORT:
					{
						// 64位导入表
						AnalysisDsImportOfPEData64(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_RESOURCE:
					{
						// 64位资源表
						AnalysisDsResourceOfPEData64(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_BASERELOC:
					{
						// 64位重定位表
						AnalysisDsBaseRelocOfPEData64(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_BOUNDIMPORT:
					{
						// 64位绑定导入表
						AnalysisDsBoundImportOfPEData64(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				case CLICK_DS_IAT:
					{
						// 64位IAT表
						AnalysisDsIATOfPEData64(GetDlgItem(hwndDlg, IDC_EDIT_DS_ALL), &g_stTLS);
						break;
					}
				} // End switch
			}

			return CALLBACK_COMPLETED;
		}
	case WM_CLOSE:
		{
			// 按下(x)按钮
			EndDialog(hwndDlg, TRUE);
			return CALLBACK_COMPLETED;
		}
	} // End switch

	return CALLBACK_UNCOMPLETED;
	
}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK DialogProcDirectory(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	/*
		用于处理"IDD_DIALOG_Directory"对话框的回调函数
	*/
	switch(uMsg)
	{
	case  WM_INITDIALOG:
		{
			// 设置小图标及大图标为同一个图标
			SetIcon(g_stTLS.m_hAppInstance, hwndDlg, IDI_ICON_ALL, IDI_ICON_ALL);

			// 分32位和64位进行进一步的PE文件解析并显示在IDD_DIALOG_Directory上
			if (FILE_OF_PE32_COMPARE(g_stTLS.m_dwGlobalFlag))
			{
				// 32位程序
				AnalysisDirectoryOfPEData32(hwndDlg, &g_stTLS);
			}
			else
			{
				// 64位程序
				AnalysisDirectoryOfPEData64(hwndDlg, &g_stTLS);
			}

			return CALLBACK_COMPLETED;
		}
	case WM_COMMAND:
		{
			// 按下各种按钮的时候发送进来的信息
			switch (LOWORD(wParam))
			{
			// 根据点击的不同按钮更改不不同标志位
			case IDC_BUTTON_D_Export:
					DsEditHandle(&g_stTLS, hwndDlg, (DWORD)&DialogProcDS_ALL, 0);
					break;
			case IDC_BUTTON_D_Import:
					DsEditHandle(&g_stTLS, hwndDlg, (DWORD)&DialogProcDS_ALL, 1);
					break;
			case IDC_BUTTON_D_Resource:
					DsEditHandle(&g_stTLS, hwndDlg, (DWORD)&DialogProcDS_ALL, 2);
					break;
			case IDC_BUTTON_D_BaseReloc:
					DsEditHandle(&g_stTLS, hwndDlg, (DWORD)&DialogProcDS_ALL, 3);
					break;
			case IDC_BUTTON_D_BoundImport:
					DsEditHandle(&g_stTLS, hwndDlg, (DWORD)&DialogProcDS_ALL, 4);
					break;
			case IDC_BUTTON_D_IAT:
					DsEditHandle(&g_stTLS, hwndDlg, (DWORD)&DialogProcDS_ALL, 5);
					break;
			}

			return CALLBACK_COMPLETED;
		}
	case WM_CLOSE:
		{
			// 按下(x)按钮
			EndDialog(hwndDlg, TRUE);

			return CALLBACK_COMPLETED;
		}
	} // End switch

	return CALLBACK_UNCOMPLETED;
}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK DialogProcSection(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	/*
		用于处理"IDD_DIALOG_Section"对话框的回调函数
	*/
	switch(uMsg)
	{
	case  WM_INITDIALOG:
		{
			// 设置小图标及大图标为同一个图标
			SetIcon(g_stTLS.m_hAppInstance, hwndDlg, IDI_ICON_ALL, IDI_ICON_ALL);
			
			// 分32位和64位进行进一步的PE文件解析并显示在dialog上
			if (FILE_OF_PE32_COMPARE(g_stTLS.m_dwGlobalFlag))
			{
				// 32位PE文件
				InitSectionListView(hwndDlg);
				AnalysisSectionOfPEData32(hwndDlg, &g_stTLS);
			}
			else
			{
				// 64位PE文件
				InitSectionListView(hwndDlg);
				AnalysisSectionOfPEData64(hwndDlg, &g_stTLS);
			}

			return CALLBACK_COMPLETED;
		}
	case WM_CLOSE:
		{
			// 按下(x)按钮
			EndDialog(hwndDlg, TRUE);

			return CALLBACK_COMPLETED;
		}
	}// End switch

	return CALLBACK_UNCOMPLETED; 
}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK DialogProcPE(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	/*
		用于处理"IDD_DIALOG_PE"对话框的回调函数
	*/
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			// 按下各种按钮的时候发送进来的信息
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON_Directory:
				{
					// 按下目录按钮
					DialogBox(g_stTLS.m_hAppInstance, MAKEINTRESOURCE(IDD_DIALOG_Directory), hwndDlg, DialogProcDirectory);
					break;
				}
			case IDC_BUTTON_Section:
				{
					// 按下区段按钮
					DialogBox(g_stTLS.m_hAppInstance, MAKEINTRESOURCE(IDD_DIALOG_Section), hwndDlg, DialogProcSection);
					break;
				}
			}
			return CALLBACK_COMPLETED;
		}
	case WM_CLOSE:
		{
			// 按下(x)按钮
			// 释放申请的空间并置零指针
			free(g_stTLS.m_fpFileBuffer);
			g_stTLS.m_fpFileBuffer = NULL;

			// 关闭此界面
			EndDialog(hwndDlg, TRUE);
			return CALLBACK_COMPLETED;
		}
	case  WM_INITDIALOG:
		{
			// 设置小图标及大图标为同一个图标
			SetIcon(g_stTLS.m_hAppInstance, hwndDlg, IDI_ICON_ALL, IDI_ICON_ALL);

			// 根据不同位数解析PE文件,将解析好的数据分别写入到此对话框中对应的编辑框中
			if (FILE_OF_PE32_COMPARE(g_stTLS.m_dwGlobalFlag))
			{
				// 32位程序
				AnalysisOfPEData32(hwndDlg, &g_stTLS);
			}
			else
			{
				// 64位程序
				AnalysisOfPEData64(hwndDlg, &g_stTLS);
			}

			return CALLBACK_COMPLETED;
		}
	} // End switch

	return CALLBACK_UNCOMPLETED;
}

//////////////////////////////////////////////////////////////////////

BOOL CALLBACK DialogProcAbout(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	/*
		用于处理"IDD_DIALOG_ABOUT"对话框的回调函数
	*/
	switch(uMsg)								
	{
	case  WM_INITDIALOG:								
		{		
			// 设置小图标及大图标 为同一个图标
			SetIcon(g_stTLS.m_hAppInstance, hwndDlg, IDI_ICON_ALL, IDI_ICON_ALL);

			return CALLBACK_COMPLETED;							
		}	
	case WM_CLOSE:
		{
			// 按下退出按钮
			EndDialog(hwndDlg, TRUE);
			return CALLBACK_COMPLETED;
		}
	} // End switch

	return CALLBACK_UNCOMPLETED;
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
			DWORD dwResult = MessageBox(hwndDlg, "确定要退出吗?", "Really?", MB_ICONQUESTION | MB_OKCANCEL);
			if (dwResult == IDOK)
			{
				// 如果结果为1则证明用户按下了确定 则退出程序
				EndDialog(hwndDlg, TRUE);
			}

			return CALLBACK_COMPLETED;
		}
	case WM_NOTIFY:
		{
			// 处理各种通用控件消息
			if (wParam == IDC_LIST_PROCESS_UPPER && ((LPNMHDR)lParam) -> code == NM_CLICK)
			{
				// 如果是鼠标左键点击 IDC_LIST_PROCESS_UPPER

				// 获取 GetPidByListViewText 的结果
				TCHAR szResult[0x20] = {0};

				// 获取用户想要查看的进程的 PID
				BOOL IsTrue = GetPidByListViewText(GetDlgItem(hwndDlg, IDC_LIST_PROCESS_UPPER), szResult);
				if (!IsTrue)
				{
					return CALLBACK_COMPLETED;
				}

				// 用于存储获取到的 PID 的值
				DWORD dwPID = NULL;

				// 使用 atoi 将获取到的字符串转换为整数
				dwPID = atoi(szResult);
				
				// 用于让操作系统显示值的结构
				LV_ITEM vitem = {0};

				// 首先删除全部的 IDC_LIST_PROCESS_LOWER 中的信息
				SendMessage(GetDlgItem(hwndDlg, IDC_LIST_PROCESS_LOWER), LVM_DELETEALLITEMS, 0, (DWORD)&vitem);

				// 遍历模块并显示在 IDC_LIST_PROCESS_LOWER 中
				EnumMoudelByProcID(GetDlgItem(hwndDlg, IDC_LIST_PROCESS_LOWER), dwPID);
			}

			return CALLBACK_COMPLETED;
		}
	case  WM_INITDIALOG:								
		{
			// 设置小图标及大图标 为同一个图标
			SetIcon(g_stTLS.m_hAppInstance, hwndDlg, IDI_ICON_ALL, IDI_ICON_ALL);

			// 初始化第一个 ListView
			InitProcessListViewUpper(hwndDlg);

			// 初始化第二个 ListView
			InitProcessListViewLower(hwndDlg);		

			// 枚举进程并显示在 ListViewUpper 上
			EnumProcByWinApi(GetDlgItem(hwndDlg, IDC_LIST_PROCESS_UPPER));

			return CALLBACK_COMPLETED;							
		}
	case  WM_COMMAND:
		{
			// 按下各种按钮的时候发送进来的信息
			switch (LOWORD(wParam))							
			{
			case IDC_BUTTON_OPEN:
				{
					// 按下"查看"按钮
					
					// 写入新的路径字符串
					DWORD dwUserChoose = PushViewButton(hwndDlg, g_stTLS.m_szFileName, "请选择一个 PE 文件");

					// 如果进行了文件选择
					if (dwUserChoose)
					{
						// 每次选择了新的文件应该先初始化有关32位及64位PE文件的标志
						FILE_OF_PE_INIT(g_stTLS.m_dwGlobalFlag);

						// 选择成功则需要判断是否是 PE 文件
						DWORD isPeFile = IsPeFile(g_stTLS.m_szFileName, &g_stTLS);

						switch (isPeFile)
						{
						case ERROR_MEMORY_REQUEST_FAILURE:
							MessageBox(hwndDlg, "内存申请失败", "请重新选择文件", MB_ICONINFORMATION | MB_ICONASTERISK);
							return CALLBACK_COMPLETED;
						case ERROR_DOS_MZ:
							MessageBox(hwndDlg, "错误的 DOS_MZ 头", "无效的 PE 文件", MB_ICONWARNING);
							return CALLBACK_COMPLETED;
						case ERROR_PE_SIGNATURE:
							MessageBox(hwndDlg, "错误的 PE 标识", "无效的 PE 文件", MB_ICONWARNING);
							return CALLBACK_COMPLETED;
						case ERROR_OPEN_FILE:
							MessageBox(hwndDlg, "文件打开失败", "请再次选择文件", MB_ICONINFORMATION | MB_ICONASTERISK);
							return CALLBACK_COMPLETED;
						case ERROR_READ_FILE:
							MessageBox(hwndDlg, "文件读取失败", "请再次选择文件", MB_ICONINFORMATION | MB_ICONASTERISK);
							return CALLBACK_COMPLETED;
						case ERROR_CLOSE_FILE:
							MessageBox(hwndDlg, "文件关闭失败", "请再次选择文件", MB_ICONINFORMATION | MB_ICONASTERISK);
							return CALLBACK_COMPLETED;
						case ERROR_PE_FILE_TOO_BIG:
							MessageBox(hwndDlg, "PE文件过大", "请重新选择文件", MB_ICONINFORMATION | MB_ICONASTERISK);
							return CALLBACK_COMPLETED;
						case FILE_OF_PE_UNKNOWN:
							MessageBox(hwndDlg, "仅支持解析32位及64位的PE文件", "请重新选择文件", MB_ICONINFORMATION | MB_ICONASTERISK);
							return CALLBACK_COMPLETED;
						}

						// 如果是有效的 PE 文件则弹出 PE 文件处理对话框
						DialogBox(g_stTLS.m_hAppInstance, MAKEINTRESOURCE(IDD_DIALOG_PE), hwndDlg, DialogProcPE);
					}

					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_Shell:
				{
					// 按下"加壳"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_TEXT:
				{
					// 按下"文字修改"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_FixIcon:
				{
					// 按下"图标修改"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_DynamicInject:
				{
					// 按下"动态注入"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_StaticInject:
				{
					// 按下"静态注入"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_MEMORY:
				{
					// 按下"内存读写"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_Reverse:
				{
					// 按下"反汇编"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_ABOUT:
				{
					// 按下"关于"按钮
					// 再弹出一个窗口 写程序说明
					DialogBox(g_stTLS.m_hAppInstance, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hwndDlg, DialogProcAbout);
					return CALLBACK_COMPLETED;
				}
			case IDC_BUTTON_HELP:
				{
					// 按下"帮助"按钮
					MessageBox(hwndDlg, "开发中", "Sorry", MB_ICONINFORMATION | MB_ICONASTERISK);
					return CALLBACK_COMPLETED;
				}
			} // End switch

		} // case WM_COMMAND
		
    } // End switch

	return CALLBACK_UNCOMPLETED;							
}

//////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(
					HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow
					)
{
 	// TODO: Place code here.

	/*
		本节练习：		
			1、画出PETools的第一个界面.		
			2、遍历进程、遍历模块
	*/

	/*
		其他说明：
			1、开发周期：5天
			2、命名要求规范
			3、遇到无法解决的问题，可以给予支持
			考核：
			练习无法完成者，跟下一期
	*/

	// 检查是否只有一个同名程序
	CheckIsExsist();

	// 将exe的句柄赋值给全局变量
	g_stTLS.m_hAppInstance = hInstance;

	// 引入需要使用的通用控件
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);   

	// 调用主对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);

	return 0;
}

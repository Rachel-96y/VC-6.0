// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// 使 DbgPrintf 仅在 debug 模式有意义
#ifdef _DEBUG
#define DbgPrintf   OutputDebugStringF
#else
#define DbgPrintf
#endif

// 通用标志位
#define CALLBACK_UNCOMPLETED				0x00000000			// 回调函数未处理
#define CALLBACK_COMPLETED					0x00000001			// 回调函数处理完成
#define ERROR_MEMORY_REQUEST_FAILURE		0x00000002			// 内存申请失败
#define ERROR_DOS_MZ						0x00000004			// 错误的DOS_MZ头标志
#define ERROR_PE_SIGNATURE					0x00000008			// 错误的PE头标志
#define ERROR_OPEN_FILE						0x00000010			// 文件打开失败
#define ERROR_READ_FILE						0x00000020			// 文件读取失败
#define ERROR_CLOSE_FILE					0x00000040			// 文件关闭失败
#define ERROR_PE_FILE_TOO_BIG				0x00000080			// PE文件过大申请空间失败
#define FILE_OF_PE_UNKNOWN					0x00000100			// 未知可执行程序
#define ERROR_CLICK							0x00000200			// 错误的点击消息
#define RETURN_FAILURE						0x00000400			// 函数执行失败
#define RETURN_SUCCESS						0x00000800			// 函数执行成功
#define FILE_MAX_SIZE						0x40000000			// 能接受的实际PE文件最大值

// 全局标志位
#define FILE_OF_PE32						0x00000001			// 32位PE文件
#define FILE_OF_PE64						0x00000002			// 64位PE文件
#define CLICK_DS_EXPORT						0x00000004			// 点击导出表按钮
#define CLICK_DS_IMPORT						0x00000008			// 点击导入表按钮
#define CLICK_DS_RESOURCE					0x00000010			// 点击资源表按钮
#define CLICK_DS_BASERELOC					0x00000020			// 点击重定位表按钮
#define CLICK_DS_BOUNDIMPORT				0x00000040			// 点击绑定导入表按钮
#define CLICK_DS_IAT						0x00000080			// 点击IAT表按钮
#define EXPORT_NAME							0x00000100			// 以名字导出

// 宏定义
#define FILE_OF_PE_INIT(i)					(i &= (~(FILE_OF_PE32 | FILE_OF_PE64 )))			// 初始化PE标志位
#define FILE_OF_PE32_COMPARE(i)				((i & FILE_OF_PE32) == FILE_OF_PE32)				// 是否是32位PE文件
#define FILE_OF_PE64_COMPARE(i)				((i & FILE_OF_PE64) == FILE_OF_PE64)				// 是否是64位PE文件
#define	FILE_OF_PE_BY_32(i)					(i |= FILE_OF_PE32)									// 32位PE标志,置1
#define	FILE_OF_PE_BY_64(i)					(i |= FILE_OF_PE64)									// 64位PE标志,置1

#define CLICK_DS_ALL_FLAG					(CLICK_DS_EXPORT				|\
											 CLICK_DS_IMPORT				|\
											 CLICK_DS_RESOURCE				|\
											 CLICK_DS_BASERELOC				|\
											 CLICK_DS_BOUNDIMPORT			|\
											 CLICK_DS_IAT)										// DS标志位

#define CLICK_DS_INIT(i)					(i &= (~CLICK_DS_ALL_FLAG))							// 初始化DS标志位
#define CLICK_DS_COMPARE(i)					(i & CLICK_DS_ALL_FLAG)								// 获取DS标志位的值
#define CLICK_DS_SELECT_EXPORT(i)			(i |= CLICK_DS_EXPORT)								// EXPORT标志位,置1
#define CLICK_DS_SELECT_IMPORT(i)			(i |= CLICK_DS_IMPORT)								// IMPORT标志位,置1
#define CLICK_DS_SELECT_RESOURCE(i)			(i |= CLICK_DS_RESOURCE)							// RESOURCE标志位,置1
#define CLICK_DS_SELECT_BASERELOC(i)		(i |= CLICK_DS_BASERELOC)							// BASERELOC标志位,置1
#define CLICK_DS_SELECT_BOUNDIMPORT(i)		(i |= CLICK_DS_BOUNDIMPORT)							// BOUNDIMPORT标志位,置1
#define CLICK_DS_SELECT_IAT(i)				(i |= CLICK_DS_IAT)									// IAT标志位,置1

#define EXPORT_FUNC_INIT(i)					(i &= (~EXPORT_NAME))								// 初始化导出函数标志位					
#define EXPORT_OF_NAME_COMPARE(i)			((i & EXPORT_NAME) == EXPORT_NAME)					// 是否是名字导出
#define EXPORT_BY_NAME(i)					(i |= EXPORT_NAME)									// 名字导出标志位,置1

#define GET_OVER_LOBYTE(i)					(i &= 0x0FFF)										// WORD抹去高4位

// TODO: reference additional headers your program requires here
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <Tlhelp32.h>					// 用于遍历进程和模块
#include <commctrl.h>					// 此模块中有通用控件
#include <Commdlg.h>					// 文件路径相关
#pragma comment(lib,"comctl32.lib")
#include "resource.h"					// 资源文件

//////////////////////////////////////////////////////////////////////
// GLOABAL
typedef class _TLSDirectory {
public:
	DWORD			m_dwGlobalFlag;				// 全局标志
	LPVOID			m_fpFileBuffer;				// 文件缓冲区指针
	HINSTANCE		m_hAppInstance;				// 自身模块句柄
	LPTSTR			m_szFileName;				// 路径字符串缓冲区
public:
	WINAPI _TLSDirectory();						// 初始化成员变量
	WINAPI _TLSDirectory(IN DWORD dwNumber);	// 路径字符大小可自定义
	WINAPI ~_TLSDirectory();					// 释放堆空间并置零指针及变量
} TLSDirectory, *PTLSDirectory;

//////////////////////////////////////////////////////////////////////
//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//
//////////////////////////////////////////////////////////////////////

VOID WINAPIV OutputDebugStringF(const char *format, ...);
//////////////////////////////////////////////////////////////////////

VOID WINAPI CheckIsExsist(void);
//////////////////////////////////////////////////////////////////////

VOID WINAPI SetListViewText(IN HWND hListProcess, 
							IN DWORD Count, 
							IN PTCHAR szNumber, 
							IN PTCHAR szString, 
							IN PTCHAR szProcessID, 
							IN PTCHAR szImageBase, 
							IN PTCHAR szSizeOfImage
							);
//////////////////////////////////////////////////////////////////////

VOID WINAPI SetListViewTextLow(IN HWND hListProcess, 
							   IN DWORD Count, 
							   IN PTCHAR szNumber, 
							   IN PTCHAR szName, 
							   IN PTCHAR szModBaseAddr, 
							   IN PTCHAR szModBaseSize, 
							   IN PTCHAR szLocal
							   );
//////////////////////////////////////////////////////////////////////

VOID WINAPI SetListViewTextOfSection(IN HWND hListSection, 
									 IN DWORD Count, 
									 IN PTCHAR szNumber, 
									 IN PTCHAR szName, 
									 IN PTCHAR szVirtualSize, 
									 IN PTCHAR szVirtualAddress, 
									 IN PTCHAR szSizeOfRawData, 
									 IN PTCHAR szPointerToRawData, 
									 IN PTCHAR szCharacteristics
									 );
//////////////////////////////////////////////////////////////////////

BOOL WINAPI GetPidByListViewText(IN HWND hwndDlg, OUT PTCHAR szResult);
//////////////////////////////////////////////////////////////////////

VOID WINAPI EnumMoudelByProcID(IN HWND hListViewLower, IN DWORD ProcessID);
//////////////////////////////////////////////////////////////////////

VOID WINAPI EnumProcByWinApi(IN HWND hListViewUpper);
//////////////////////////////////////////////////////////////////////

VOID WINAPI InitProcessListViewUpper(IN HWND hwndDlg);
//////////////////////////////////////////////////////////////////////

VOID WINAPI InitProcessListViewLower(HWND hwndDlg);
//////////////////////////////////////////////////////////////////////

VOID WINAPI InitSectionListView(HWND hwndDlg);

//////////////////////////////////////////////////////////////////////

DWORD WINAPI SetIcon(IN HINSTANCE hAppInstance, IN HWND hwndDlg, IN DWORD IconSmall, IN DWORD IconBig);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI PushViewButton(IN HWND hwndDlg, IN LPTSTR TEXT(szFileName), IN PTCHAR TEXT(szTitle));
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisOfPEData32(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisOfPEData64(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDirectoryOfPEData32(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDirectoryOfPEData64(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisSectionOfPEData32(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisSectionOfPEData64(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI DsEditHandle(IN PTLSDirectory g_pStTLS, IN HWND hwndDlg, IN DWORD dwCallBackFunction, IN DWORD dwFlag);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsExportOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsExportOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsImportOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsImportOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsResourceOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsResourceOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBaseRelocOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBaseRelocOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBoundImportOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBoundImportOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsIATOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsIATOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS);
//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_STDAFX_H__A89DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
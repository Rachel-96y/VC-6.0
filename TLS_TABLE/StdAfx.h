// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E4C5A544_BD5D_44CC_85E9_64E49236BEAC__INCLUDED_)
#define AFX_STDAFX_H__E4C5A544_BD5D_44CC_85E9_64E49236BEAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// #define PATH "E:\\study\\C_X86\\C_X86_2010\\test_2\\Release\\test_2.exe"
#define PATH "E:\\study\\C_X86\\test\\Win32Api_15\\Release\\Win32Api_15.exe"

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

// TODO: reference additional headers your program requires here
//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 此函数接收一个 Rva 及一个 FileBuffer 指针 返回一个 Foa
//
// 参数说明:
// Rva							在内存中的偏移
// pFileBuffer					FileBuffer 缓冲区指针
//
// 返回值说明:
// 如果执行成功则返回该值在文件中的偏移 失败则返回0
//////////////////////////////////////////////////////////////////////

DWORD ChangeRvaToFoa64(IN DWORD RVA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// 同ChangeRvaToFoa32
//////////////////////////////////////////////////////////////////////


DWORD ChangeFoaToRva32(IN UINT FOA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 此函数接收一个 Foa 及一个 FileBuffer 指针 返回一个 Rva
//
// 参数说明:
// Foa							在文件中的偏移
// pFileBuffer					FileBuffer 缓冲区指针
//
// 返回值说明:
// 如果执行成功则返回该值在文件中的偏移 失败则返回0
//////////////////////////////////////////////////////////////////////

DWORD ChangeFoaToRva64(IN UINT FOA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// 同ChangeFoaToRva32位
//////////////////////////////////////////////////////////////////////

DWORD IsPeFile(IN LPTSTR lpszFilePath, OUT LPVOID* lpFileBuffer);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 根据传入的文件缓冲区 检测文件是否是PE文件
//
// 参数说明:
// szFilePath					文件在内存中的缓冲区指针
// g_pStTLS								
// 
// 返回值说明:
// 如果是PE文件则返回RETURN_SUCESS
// 如果返回ERROR_DOS_MZ则代表是错误的DOS_MZ头
// 如果返回ERROR_PE_SIGNATURE则代表是错误的PE标识
//////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_STDAFX_H__E4C5A544_BD5D_44CC_85E9_64E49236BEAC__INCLUDED_)

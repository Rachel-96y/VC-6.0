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

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define CALLBACK_COMPLETED		TRUE
#define CALLBACK_UNCOMPLETED	FALSE
#define RETURN_SUCCESS			TRUE
#define RETURN_FAILED			FALSE

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

void WINAPIV OutputDebugStringF(const char *format, ...);
//////////////////////////////////////////////////////////////////////

HANDLE WINAPI CheckIsExsist(void);
//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)

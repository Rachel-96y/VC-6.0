// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__771F1359_D9FD_4CB2_AF94_D9B240E5CE34__INCLUDED_)
#define AFX_STDAFX_H__771F1359_D9FD_4CB2_AF94_D9B240E5CE34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>



// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

int __stdcall Plus(int x,int y);
int __stdcall Sub(int x,int y);
int __stdcall Mul(int x,int y);	
int __stdcall Div(int x,int y);	

#endif // !defined(AFX_STDAFX_H__771F1359_D9FD_4CB2_AF94_D9B240E5CE34__INCLUDED_)

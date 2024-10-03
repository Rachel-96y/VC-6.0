// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F68CAC25_6E00_43F9_BEFA_A0D11804E96E__INCLUDED_)
#define AFX_STDAFX_H__F68CAC25_6E00_43F9_BEFA_A0D11804E96E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include<windows.h>
#include <iostream>
#include <stdio.h>


// TODO: reference additional headers your program requires here


//{{AFX_INSERT_LOCATION}}

//打开文件 并申请内存
//参数OUT PVOID* pFileBuffer 传出PVOID类型的 pFileBuffer指针
//返回值为  缓存的长度
DWORD CreateFileBuffer(IN LPSTR lpszFile, OUT PVOID* pFileBuffer);
//打印所有头的信息
//参数 传入缓存FileBuffer指针
void ToLoaderPE(IN PVOID pFileBuffer);

//把FileBuffer缓存内容拉伸至ImageBuffer
//参数1 传入FileBuffer首地址指针
//参数2 传出ImageBuffer首地址指针
//返回值 ImageBuffer的长度
DWORD CopyFileBufferToImageBuffer(IN PVOID pFileBuffer, OUT PVOID* pImageBuffer);

//ImageBuffer缓存内容按文件格式拷贝至NewBuffer
//参数1 传入ImageBuffer首地址指针
//参数2 传出NewBuffer首地址指针
//返回值 NewBuffer的长度
DWORD ImageBufferToNewBuffer(IN PVOID ImageBuffer, OUT PVOID* pNewBuffer);

//文件存盘
//参数1 传入NewBuffer的指针
//参数2 传入NewBuffer 长度
//返回值 成功返回1 失败返回0;
DWORD WriteMemeryToFile(IN LPVOID NewBuffer, IN DWORD file_size);

//RVA 转换FOA
void convertFoaToRva(IN LPSTR lpszFile);


//void ChangeImageBufferCode(IN LPSTR lpszFile, IN BYTE* CodeSize);
void ChangeImageBufferCode1(IN LPSTR lpszFile, IN BYTE* ShellCode, IN DWORD Codesize, IN DWORD Maddress);
 


// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F68CAC25_6E00_43F9_BEFA_A0D11804E96E__INCLUDED_)

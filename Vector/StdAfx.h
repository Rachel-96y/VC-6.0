// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2CA63EA4_5C33_4AEF_A008_DAC3396AB0B6__INCLUDED_)
#define AFX_STDAFX_H__2CA63EA4_5C33_4AEF_A008_DAC3396AB0B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// ----------------------------- 引用的头文件 -----------------------------



#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include <stdlib.h>



// ----------------------------- 声明的全局变量 -----------------------------

// 定义的路径: 

// 声明的常量: 


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


// ----------------------------- 声明的函数 -----------------------------



// ============================================================================
LPVOID CreateFileBuffer(IN LPSTR pPeFilePath);
/*
函数功能说明:
根据给定的文件路径 读取硬盘上的文件 获取文件大小
根据文件大小申请一块相同大小的内存空间
由此申请的空间不在此函数进行释放

参数说明:
pPeFilePath 指向文件路径的指针

返回值说明:
如果执行成功则返回已经申请的空间的首地址指针 失败则返回0 
*/
// ****************************************************************************


// ============================================================================
DWORD MyAlignment(IN LPVOID pPeMemory, IN UINT FileOrSectionAlignment,IN UINT SizeOfCode);
/*
函数功能说明:
根据提供的 pPeMemory 及 需要对齐的字节数 及 对齐方式来计算出对齐后的结果

参数说明:
pPeMemory					PE 文件在内存中的首地址
FileOrSectionAlignment		指定0为文件对齐 1为内存对齐
CodeSize					需要对齐的代码的字节数

返回值说明:
如果执行成功则返回对齐后的结果 失败则返回0
*/ 
// ****************************************************************************

// ============================================================================
DWORD ReadPeFileToFileBuffer(IN LPSTR lpszFile, OUT LPVOID pFileBuffer);
/*
函数功能说明:
根据给定的文件路径 读取硬盘上的文件
将其读取进指定的内存空间

参数说明:
pPeFilePath		硬盘文件路径
pFileBuffer		FileBuffer 缓冲区指针

返回值说明:
如果函数执行成功则返回读取的文件的大小(字节) 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
DWORD WriteMemeryToFile(IN LPVOID pMemBuffer, IN UINT SizeOfFile, OUT LPSTR lpszFile);
/*
函数功能说明:
根据给定的 pMemBuffer 及文件路径和所要写入的文件大小
将内存中的数据写入到磁盘上

参数说明:
pMemBuffer		内存缓冲区指针
SizeOfFile		要写入的文件大小
lpszFile		要写入磁盘的路径

返回值说明:
如果存盘成功则返回写入数据的大小 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
DWORD ChangeRvaToFoa(IN UINT RVA, IN LPVOID pFileBuffer);
/*
函数功能说明:
此函数接收一个 Rva 及一个 FileBuffer 指针 返回一个 Foa

参数说明:
Rva				在内存中的偏移
pFileBuffer		FileBuffer 缓冲区指针

返回值说明:
如果执行成功则返回该值在文件中的偏移 失败则返回0
*/ 
// ****************************************************************************



// ============================================================================
DWORD ChangeFoaToRva(IN UINT FOA, IN LPVOID pFileBuffer);
/*
函数功能说明:
此函数接收一个 Foa 及一个 FileBuffer 指针 返回一个 Rva

参数说明:
Foa				在文件中的偏移
pFileBuffer		FileBuffer 缓冲区指针

返回值说明:
如果执行成功则返回该值在文件中的偏移 失败则返回0
*/ 
// ****************************************************************************



// ============================================================================
LPVOID GetFunctionAddrByName(IN LPVOID pFileBuffer, IN LPSTR FunctionName);
/*
函数功能说明:
根据名字获取函数在模块中的地址并返回

参数说明:
pFileBuffer		FileBuffer指针
FunctionName	函数名指针

返回值说明:
函数执行成功则返回所找函数的地址 失败则返回0
*/ 
// ****************************************************************************



// ============================================================================
LPVOID GetFunctionAddrByOrdinals(IN LPVOID pFileBuffer, IN UINT FunctionOrdinal);
/*
函数功能说明:
根据序号获取函数在模块中的地址并返回

参数说明:
pFileBuffer				FileBuffer 指针
FunctionNameOrdinal		函数序号(2个字节的宽度)

返回值说明:
函数执行成功则返回所找函数的地址 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
DWORD SizeOfDllFunctionLength(IN LPSTR DLL_PATH);
/*
函数功能说明:
根据给定的 DLL 路径获取其中的第一个函数名的长度

参数说明:
DLL_PATH				DLL 路径

返回值说明:
函数执行成功则返回第一个函数名称的长度 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
LPVOID CleanPeData(IN LPVOID pFileBuffer, IN UINT SizeOfFileBuffer);
/*
函数功能说明:
如果文件最后存在不加载到内存中的数据则重新创建一个 NewFileBuffer
将 FileBuffer 中需要加载到内存中的数据拷贝过去 并释放 FileBuffer 空间

参数说明:
pFileBuffer				FileBuffer 指针
SizeOfFileBuffer		FileBuffer 大小

返回值说明:
如果重新申请了空间 则返回 pFileBuffer 否则返回 pNewFileBuffer 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
DWORD SizeOfCodeToInjectDll(IN LPVOID pFileBuffer, IN LPSTR DLL_PATH, IN UINT FunctionNameLength);
/*
函数功能说明:
计算注入一个 DLL 至少需要多少空间
需要移动的全部导入表 + 新增的导入表 + 全0结构的导入表 + IAT + INT + _IMAGE_IMPORT_BY_NAME
最终的代码字节数 = 所有 DLL 的导入表大小(包含空白结构的 20 个字节大小) + 新增的 20 个字节的导入表大小 + IAT + INT + (Dll名长度 + 1) + (函数名长度 + 2 + 1)
参数说明:
pFileBuffer				FileBuffer 指针 ( EXE 的 pFileBuffer )
DLL_PATH				需要注入的 DLL 的路径
FunctionNameLength		需要注入的 DLL 里的函数的长度

返回值说明:
如果执行成功则返回 需要的空间大小(未进行文件对齐前) 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
LPVOID IsEnoughSpace(IN LPVOID pFileBuffer, IN UINT SizeOfCodeToInjectDll);
/*
函数功能说明:
判断是否有足够的空间写入代码(各类表)

参数说明:
pFileBuffer				FileBuffer 指针 ( EXE 的 pFileBuffer )
SizeOfCodeToInjectDll	需要写入的代码的大小

返回值说明:
如果有空间进行写入则返回要开始写入的位置的指针
*/ 
// ****************************************************************************


// ============================================================================
DWORD ImportDescriptorInjectDll(IN LPVOID pFileBuffer, IN LPVOID pCodeBegin, IN LPCSTR strDllFunctionNameAddr, IN LPSTR FileNameAddr, IN DWORD SizeOfFileBufferEx);
/*
函数功能说明:
根据给定的 pFileBuffer 以及 pCodeBegin 新增一个导入表
从而实现 DLL 注入的效果

参数说明:
pFileBuffer				FileBuffer 指针 (EXE 的 pFileBuffer)
pCodeBegin				要进行注入的代码从哪里开始写入的指针
strDllFunctionNameAddr	DLL 中以名称导出的第一个函数的名称的地址
FileNameAddr			模块名称
SizeOfFileBufferEx	为变化前的 FileBuffer 大小

返回值说明:
执行成功则返回1 失败且 pCodeBegin 为 NULL 则返回0 已经执行过一次则返回 -1
*/ 
// ****************************************************************************


// ============================================================================
DWORD SizeOfFileBuffer(IN LPVOID pFileBuffer);
/*
函数功能说明:
根据 FileBuffer 计算出当前 FileBuffer 的大小

参数说明:
pFileBuffer				FileBuffer 指针 ( EXE 的 pFileBuffer )

返回值说明:
如果执行成功则返回当前的 FileBuffer 的大小 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
LPVOID ExpandSection(IN UINT IsSuccess, IN LPVOID pFileBuffer, IN UINT SizeOfCodeAlignment);
/*
函数功能说明:
根据对齐参数的大小扩大节

参数说明:
IsSuccess				为 NULL 则证明不需要扩大节
pFileBuffer				FileBuffer 指针 ( EXE 的 pFileBuffer )
SizeOfCodeAlignment		需要写入的数据进行文件对齐后的大小

返回值说明:
如果执行成功则返回 pNewFileBuffer 指针 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
LPSTR DllFunctionName(IN LPSTR DLL_PATH);
/*
函数功能说明:
根据给定的 DLL 获取到第一个函数的地址

参数说明:
DLL_PATH				DLL 在磁盘上的路径

返回值说明:
如果执行成功则返回 DLL中函数的地址 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
LPSTR GetFileName(IN LPSTR FilePath);
/*
函数功能说明:
根据传入的文件路径获取文件名称并返回

参数说明:
FilePath				文件在磁盘上的路径

返回值说明:
如果执行成功则返回文件名称 失败则返回0
*/ 
// ****************************************************************************



#endif // !defined(AFX_STDAFX_H__2CA63EA4_5C33_4AEF_A008_DAC3396AB0B6__INCLUDED_)
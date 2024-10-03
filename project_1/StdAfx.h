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
#include <memory.h>



// ----------------------------- 声明的全局变量 -----------------------------

// 这是我的计算机上的 MessageBoxA 的地址
// 因为这不是 ShellCode 故在其它计算机上这个值会发生变化
DWORD ToMessageBoxA = 0x76691610;

// 定义的两个路径: PATH 为目标PE程序路径	NEW_PATH 为新生成的添加代码后的程序路径
#define PATH "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\notepad.exe"
#define NEW_PATH "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\NewNotepad.exe"



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
LPVOID CreateImageBuffer(IN LPVOID pFileBuffer);
/*
函数功能说明:
根据给定的 pFileBuffer 指针读取内存中的内容
需要先判断是否是PE文件
如果是PE文件则根据 可选PE头中 SizeOfImage 的大小来申请内存空间

参数说明:
pFileBuffer 指向 FileBuffer 内存缓冲区的指针

返回值说明:
如果执行成功则返回所申请的空间的指针 pImageBuffer 失败则返回0
*/
// ****************************************************************************


// ============================================================================
LPVOID CreateNewBuffer(IN LPVOID pImageBuffer);
/*
函数功能说明:
根据给定的 ImageBuffer 计算得出所压缩后的 NewBuffer 的空间大小
并根据此大小申请对应的空间

参数说明:
pImageBuffer	指向 ImageBuffer 内存缓冲区的指针

返回值说明:
如果执行成功则返回所申请的空间的指针 pNewBuffer 失败则返回0
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
DWORD CopyFileBufferToImageBuffer(IN LPVOID pFileBuffer,OUT LPVOID pImageBuffer);
/*
函数功能说明:
根据给定的 pFileBuffer 和 pImageBuffer 进行数据的复制
需要先判断是否是PE文件
如果是PE文件 则根据所给的指针 从 FileBuffer 将数据复制到 ImageBuffer 中

参数说明:
pFileBuffer		FileBuffer 缓冲区指针
pImageBuffer	ImageBuffer 缓冲区指针

返回值说明:
如果拷贝成功则返回所拷贝的字节数大小 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
DWORD CopyImageBufferToNewBuffer(IN LPVOID pImageBuffer,OUT LPVOID pNewBuffer);
/*
函数功能说明:
根据给定的 pImageBuffer 和 pNewBuffer 进行数据的复制
需要先判断是否是PE文件
如果是PE文件 则根据所给的指针 从 ImageBuffer 将数据复制到 NewBuffer 中

参数说明:
pImageBuffer	ImageBuffer 缓冲区指针	
NewBuffer		NewBuffer	缓冲区指针	
	
返回值说明:
如果拷贝成功则返回所拷贝的字节数大小 失败则返回0
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
BOOL AddCodeToImageBuffer(IN PUCHAR pCodeArray,IN UINT HowNumberOfSections, IN UINT SizeOfArray, OUT LPVOID pImageBuffer);
/*
函数功能说明:
根据给定的 指向数组的指针 及 pImageBuffer指针 进行代码的插入
需要同时指定将代码插入到哪一个节中 并指明数组的大小

参数说明:
pCodeArray				指向数组的的指针
HowNumberOfSections		要插入哪个节(从1开始)
pImageBuffer			指向 ImageBuffer 的缓冲区指针
SizeOfArray				数组长度

返回值说明:
如果代码插入成功返回TRUE 失败则返回0
*/ 
// ****************************************************************************



#endif // !defined(AFX_STDAFX_H__2CA63EA4_5C33_4AEF_A008_DAC3396AB0B6__INCLUDED_)



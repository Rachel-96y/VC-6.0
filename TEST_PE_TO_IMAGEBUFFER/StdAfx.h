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

// 这是我的计算机上的 MessageBoxA 的地址 每次关机后这个值都会发生变化
// 因为这不是 ShellCode 故在其它计算机上这个值会发生变化
DWORD ToMessageBoxA = 0x77B509A0;

// 统一定义 数组内代码的长度
#define CODE_LENGTH 18

// 定义的两个路径: PATH 为目标PE程序路径	NEW_PATH 为新生成的添加代码后的程序路径

#define PATH "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\NewDynamicLink.dll"
#define NEW_PATH "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\IMAGE_DynamicLink.dll"



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
LPVOID CreateNewImageBuffer(IN LPVOID pImageBuffer, IN UINT SizeOfCode);
/*
函数功能说明:
创建新增节所需要的空间
根据给定的 pImageBuffer 指针 计算得出是否有足够的空间添加一个节表 
如果有足够的空间添加一个节表 则根据所提供的 SizeOfCode 的大小
计算得出其内存对齐后 整个 PE 文件在内存中的总大小 并以此申请相同大小的空间

参数说明:
pImageBuffer			指向 ImageBuffer 内存缓冲区的指针	
SizeOfSectionNumber		所要添加的代码的长度

返回值说明:
如果执行成功则返回所申请的空间的指针 pNewImageBuffer 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
LPVOID CreateNewImageBufferEx(IN LPVOID pImageBuffer, IN UINT Size);
/*
函数功能说明:
根据所提供的 pImageBuffer 及 Size 来申请一块空间

参数说明:
pImageBuffer	指向 ImageBuffer 缓冲区
Size			要新增的代码内存对齐后的大小

返回值说明:
如果执行成功则返回所申请的空间的指针 pNewImageBuffer 失败则返回0
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
此函数用于静态注入代码
根据给定的 指向数组的指针 及 pImageBuffer指针 进行代码的插入
需要同时指定将代码插入到哪一个节中 并指明数组的大小

参数说明:
pCodeArray				指向数组的的指针
HowNumberOfSections		要插入哪个节(从1开始)
SizeOfArray				数组长度
pImageBuffer			指向 ImageBuffer 的缓冲区指针

返回值说明:
如果代码插入成功返回TRUE 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
BOOL CopyImageBufferToNewImageBuffer(IN LPVOID pImageBuffer, OUT LPVOID pNewImageBuffer, IN UINT SizeOfCode, IN UINT IsExistMisc);
/*
函数功能说明:
此函数用于新增节
将 ImageBuffer 的数据复制到 NewImageBuffer 中
校对所有需要修正的PE头里的值 并向上移动PE头和节表

参数说明:
pImageBuffer		ImageBuffer 缓冲区指针
pNewImageBuffer		NewImageBuffer 缓冲区指针
SizeOfCode			未对其前的代码大小
IsExistMisc			是否存在 Misc 的值 0代表无 输入其它值则代表此值等于SizeOfRawData

返回值说明:
如果执行成功则返回TRUE 失败则返回0
*/ 
// ****************************************************************************


// ============================================================================
BOOL CopyImageBufferToNewImageBufferEx(IN LPVOID pImageBuffer, OUT LPVOID pNewImageBuffer, IN UINT AlignNumber, IN UINT IsExistMisc, IN UINT CodeSize);
/*
函数功能说明:
此函数用于扩大节
将 ImageBuffer 的数据复制到 NewImageBuffer 中
校对所有需要修正的PE头里的值

参数说明:
pImageBuffer		ImageBuffer 缓冲区指针
pNewImageBuffer		NewImageBuffer 缓冲区指针
AlignNumber			要扩大的节对齐后的大小
IsExistMisc			是否存在 Misc 的值 0代表无 1代表值为 SizeOfRawData - CodeSize 输入其它值则代表此值等于SizeOfRawData
CodeSize			指定从节的哪里开始写入数据 这个值改为要注入的代码的长度即可

返回值说明:
如果执行成功则返回TRUE 失败则返回0
*/ 
// ****************************************************************************



// ============================================================================
BOOL ChangeImageBufferData(IN LPVOID pImageBuffer, IN UINT IsExistMisc, IN UINT CodeSize);
/*
函数功能说明:
此函数用于合并节
将 ImageBuffer 的数据全部修正

参数说明:
pImageBuffer		指向 ImageBuffer 缓冲区指针
IsExistMisc			是否存在 Misc 的值 0代表无 1代表值为 SizeOfRawData - CodeSize 输入其它值则代表此值等于SizeOfRawData
CodeSize			指定从节的哪里开始写入数据 这个值改为要注入的代码的长度即可

返回值说明:
如果执行成功则返回TRUE 失败则返回0
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
pFileBuffer				FileBuffer指针
FunctionNameOrdinal		函数序号(2个字节的宽度)

返回值说明:
函数执行成功则返回所找函数的地址 失败则返回0
*/ 
// ****************************************************************************




#endif // !defined(AFX_STDAFX_H__2CA63EA4_5C33_4AEF_A008_DAC3396AB0B6__INCLUDED_)
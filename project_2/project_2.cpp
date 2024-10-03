#include "stdafx.h"

/*

练习：

1、手动新增一个节表和节，保证修改后的程序能正确执行.

2、编程实现：新增一个节，并添加代码.

*/



int main(int argc, char* argv[])
{
	/*
		新增一个节表和节，保证修改后的程序能正确执行
		编程实现：新增一个节，并添加代码.
	*/

	// 定义 ShellCode 数组
	UCHAR CodeArray[18] = 
	{	
		0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00,  
		0xE8, 0x00, 0x00, 0x00, 0x00, 
		0xE9, 0x00, 0x00, 0x00, 0x00,
	}; 

	// 根据文件大小申请空间
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将文件读进指定内存
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 根据 FileBuffer 创建 ImageBuffer
	LPVOID pImageBuffer = CreateImageBuffer(pFileBuffer);

	// 从 FileBuffer 拷贝到 ImageBuffer
	CopyFileBufferToImageBuffer(pFileBuffer, pImageBuffer);

	// 根据需要添加的代码大小 申请一块 NewImageBuffer 空间
	// (需要先判断是否有空间添加1个节表)
	// (总共申请的内存空间为 新增一个节数据后内存对齐的大小)
	LPVOID pNewImageBuffer = CreateNewImageBuffer(pImageBuffer, 18);

	//将数据从 ImageBuffer 拷贝到 NewImageBuffer 再新增一个节表和一个节 校准全部数据 
	CopyImageBufferToNewImageBuffer(pImageBuffer, pNewImageBuffer, 18, 0);

	// 向新增的节的区域添加代码
	DWORD result = AddCodeToImageBuffer(&CodeArray[0], 4, 18, pNewImageBuffer);
	if (!result)
	{
		return NULL;
	}

	// 根据 NewImageBuffer 计算得出 NewBuffer的大小 申请对应空间

	LPVOID pNewBuffer = CreateNewBuffer(pNewImageBuffer);

	// 从 NewImageBuffer 还原到 NewBuffer (使用 CopyImageBufferToNewBuffer 函数可以达到要求)
	DWORD SizeOfCopyByte = CopyImageBufferToNewBuffer(pNewImageBuffer, pNewBuffer);

	// 存盘
	WriteMemeryToFile(pNewBuffer, SizeOfCopyByte, NEW_PATH);
	
 	return 0;
}
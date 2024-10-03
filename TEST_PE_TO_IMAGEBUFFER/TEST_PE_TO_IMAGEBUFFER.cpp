// TEST_PE_TO_IMAGEBUFFER.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	/* 拉伸 PE 文件并存盘 */

	// 读取一个文件 根据大小申请空间
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 读取硬盘上的文件到指定内存空间 返回读取的大小
	DWORD SizeOfReadFileToFileBuffer = ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 根据 FileBuffer 中的 SizeOfImage 成员大小申请一块 ImageBuffer
	LPVOID pImageBuffer = CreateImageBuffer(pFileBuffer);

	// 将 PE 文件 从 FileBuffer 拉伸并放入  ImageBuffer
	DWORD SizeOfCopyFileBufferToImageBuffer = CopyFileBufferToImageBuffer(pFileBuffer, pImageBuffer);
	
	// 将 NewBuffer 中的内容存储至指定的路径
	DWORD SizeOfWriteDate = WriteMemeryToFile(pImageBuffer, SizeOfCopyFileBufferToImageBuffer, NEW_PATH);

	getchar();

	return 0;
}
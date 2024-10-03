#include "stdafx.h"

/*

练习：
1、向代码节添加代码,编程实现
2、向其它节空白区添加代码,编程实现

*/


int main(int argc, char* argv[])
{
	/* 向代码节添加代码 && 向其他节空白区添加代码 */

	// 先定义代码数组 后期对代码进行校对 此数组共 18 个字节
	UCHAR CodeArray[18] = 
	{	
		0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00,  
		0xE8, 0x00, 0x00, 0x00, 0x00, 
		0xE9, 0x00, 0x00, 0x00, 0x00,
	};  

	// 读取一个文件 根据大小申请空间
	LPVOID pFileBuffer = CreateFileBuffer(PATH);
	if (!pFileBuffer)
	{
		printf("文件读取失败");
		free(pFileBuffer);
		pFileBuffer = NULL;
		getchar();
		return NULL;
	}

	// 读取硬盘上的文件到指定内存空间 返回读取的大小
	DWORD SizeOfReadFileToFileBuffer = ReadPeFileToFileBuffer(PATH, pFileBuffer);
	if (!SizeOfReadFileToFileBuffer)
	{
		printf("文件读取失败");
		free(pFileBuffer);
		pFileBuffer = NULL;
		getchar();
		return NULL;
	}

	// 根据 FileBuffer 中的 SizeOfImage 成员大小申请一块 ImageBuffer
	LPVOID pImageBuffer = CreateImageBuffer(pFileBuffer);
	if (!pImageBuffer)
	{
		printf(" ImageBuffer 空间申请失败");
		free(pFileBuffer);
		pFileBuffer = NULL;
		getchar();
		return NULL;
	}

	// 将 PE 文件 从 FileBuffer 拉伸并放入  ImageBuffer
	DWORD SizeOfCopyFileBufferToImageBuffer = CopyFileBufferToImageBuffer(pFileBuffer, pImageBuffer);
	if (!SizeOfCopyFileBufferToImageBuffer)
	{
		printf("从 FileBuffer 到 ImageBuffer 复制失败");
		free(pFileBuffer);
		free(pImageBuffer);
		pFileBuffer = NULL;
		pImageBuffer = NULL;
		getchar();
		return NULL;
	}

	// 向代码节添加代码  
	BOOL IsTrue = AddCodeToImageBuffer(&CodeArray[0], 1, 18, pImageBuffer);
	if (IsTrue == NULL)
	{
		printf("添加代码失败!");
		free(pFileBuffer);
		free(pImageBuffer);
		pFileBuffer = NULL;
		pImageBuffer = NULL;
		getchar();
		return NULL;
		
	}

	// 根据 ImageBuffer 中的值 申请一块 NewBuffer 空间
	LPVOID pNewBuffer = CreateNewBuffer(pImageBuffer);
	if (!pNewBuffer)
	{
		printf("根据 pImageBuffer 申请 NewBuffer 空间失败");
		free(pFileBuffer);
		free(pImageBuffer);
		pFileBuffer = NULL;
		pImageBuffer = NULL;
		getchar();
		return NULL;
	}

	// 根据 pImageBuffer 和 pNewBuffer 将 ImageBuffer 中的数据拷贝到 NewBuffer 中
	DWORD SizeOfCopyImageBufferToNewBuffer = CopyImageBufferToNewBuffer(pImageBuffer, pNewBuffer);
	if (!SizeOfCopyImageBufferToNewBuffer)
	{
		printf("从 ImageBuffer 拷贝数据到 NewBuffer 失败");
		free(pFileBuffer);
		free(pImageBuffer);
		free(pNewBuffer);
		pNewBuffer = NULL;
		pFileBuffer = NULL;
		pImageBuffer = NULL;
		getchar();
		return NULL;
	}
	
	// 将 NewBuffer 中的内容存储至指定的路径
	DWORD SizeOfWriteDate = WriteMemeryToFile(pNewBuffer, SizeOfCopyImageBufferToNewBuffer, NEW_PATH);
	if (SizeOfWriteDate == NULL)
	{
		printf("存盘失败!");
		free(pFileBuffer);
		free(pImageBuffer);
		free(pNewBuffer);
		pNewBuffer = NULL;
		pFileBuffer = NULL;
		pImageBuffer = NULL;
		getchar();
		return NULL;
	}

	// 成功执行后需要释放空间
	printf("存盘成功!\n");
	free(pFileBuffer);
	free(pImageBuffer);
	free(pNewBuffer);
	pNewBuffer = NULL;
	pFileBuffer = NULL;
	pImageBuffer = NULL;

	getchar();

	return 0;
}

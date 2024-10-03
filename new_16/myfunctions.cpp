#include "stdafx.h"
#include "myfunctions.h"
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>

Data FileData;  // 定义一个存储文件的数据的变量 (文件大小, 源文件指针, 目标文件指针)

Data* GetFileSize(char* PATH)
{
	/* 打开文件流 获取文件大小 并返回 类型: unsigned int */

	_iobuf* pFileOpen = fopen(PATH, "rb"); // 以只读二进制模式打开文件流  

	if (pFileOpen == NULL)  
	{
		// 返回值为NULL则函数fopen执行出现错误
		exit(0);
	}

	int result = fseek(pFileOpen, 0, SEEK_END);

	if (result)
	{
		// 返回值不为0则fseek执行失败
		exit(0);
	}

	int FileSize = (int)ftell(pFileOpen);

	if (FileSize == -1L)
	{
		// 返回值为-1L则ftell执行失败
		exit(0);
	}

	FileData.SizeOfFile = FileSize;   // 向FileData中的成员SizeOfFile写入FileSize

	FileData.FileStream = pFileOpen;  // 向FileData中的成员FileStream写入pFileOpen

	Data* pFileData = (Data*)&FileData;   // 获取FileData(全局变量)的地址并返回

	return pFileData;
}

int CloseFileStream(_iobuf* pFileOpen)
{
	/* 此函数用于关闭文件流 */
	int ReturnValue = fclose(pFileOpen);  

	if (ReturnValue == EOF)
	{
		// 文件流未成功关闭
		exit(0);
	}

	return 0;
}

char* RequestMemory(int fileSize)
{
	/* 
	根据所需大小申请堆区空间
	返回值是char*类型
	注意: 此函数没有手动释放所申请的内存
	*/

	char* pMemory = (char*)malloc(sizeof(char) * fileSize);

	if (pMemory == NULL)
	{
		// 内存申请失败
		pMemory = NULL;

		return pMemory;
	}

	memset(pMemory, 0xCCCCCCCC, sizeof(char) * fileSize);  // 申请的内存初始化
	
	return pMemory;
}

void FreeMemory(char* pMemory)
{
	/* 释放所申请的堆区内存空间 */

	free(pMemory);  // 释放内存

	pMemory = NULL;  // 初始化指针
}

char* ReadFileToMemory(char* pMemory, unsigned int SizeOfFile,  \
					   unsigned int ReadSizeMax, _iobuf* pFileStream)
{
	/* 读取文件流到指定的堆区内存空间并返回内存编号 */

	fseek(pFileStream, 0, SEEK_SET);  // 将文件指针设置到开头

	unsigned int result = fread(pMemory, SizeOfFile, ReadSizeMax, pFileStream);
	
	if (result - ReadSizeMax)
	{
		pMemory = NULL;
	}

	return pMemory;
}

int WriteMemoryToNewFile(char* pMemory, unsigned int SizeOfFile, \
						 unsigned int ReadSizeMax, char* PATH)
{
	/* 从流中读取数据写入到硬盘指定位置 */

	_iobuf* pNewFileOpen = fopen(PATH, "wb");

	if (pNewFileOpen == NULL)  
	{
		// 返回值为NULL则函数fopen执行出现错误
		exit(0);
	}

	unsigned int result = fwrite(pMemory, SizeOfFile, ReadSizeMax, pNewFileOpen);

	if (result - ReadSizeMax)
	{
		return 1;
	}

	FileData.WriteFileStream = pNewFileOpen;

	return 0;
}

void PrintFormatOfPe(unsigned int number, char* pMemoryAddress)
{
	/* 

	此函数用于打印PE文件的数据 
	以十六进制的格式显示
	此函数必须在堆空间及 FileData 结构体有数据的情况下才可以使用
	需要传入 FileData 中的 SizeOfFile 成员

	*/

	for (unsigned int i = 0; i < number; i++)
	{

		if (i != 0 && i % 0x10 == 0)
		{
			printf("\n");
		}

		switch(*(pMemoryAddress + i))
		{
			case 0x00:
				printf("00    ");
				break;
			case 0x01:
				printf("01    ");
				break;
			case 0x02:
				printf("02    ");
				break;
			case 0x03:
				printf("03    ");
				break;
			case 0x04:
				printf("04    ");
				break;
			case 0x05:
				printf("05    ");
				break;
			case 0x06:
				printf("06    ");
				break;
			case 0x07:
				printf("07    ");
				break;
			case 0x08:
				printf("08    ");
				break;
			case 0x09:
				printf("09    ");
				break;
			case 0x0A:
				printf("0a    ");
				break;
			case 0x0B:
				printf("0b    ");
				break;
			case 0x0C:
				printf("0c    ");
				break;
			case 0x0D:
				printf("0d    ");
				break;
			case 0x0E:
				printf("0e    ");
				break;
			case 0x0F:
				printf("0f    ");
				break;
			default:
				printf("%x    ", (unsigned char)*(pMemoryAddress + i));
		}
	}
}
#include "stdafx.h"
#include "myfunctions.h"

/*
本节练习：						
				
			
1、用十六进制文本编辑器，打开一个记事本的.exe文件，再打开在内存中的记事本						
进程，记录下这两个文件的不同.						
			
2、将记事本的.exe文件读取到内存，并返回读取后在内存中的地址.						
				
3、将内存中的数据存储到一个文件中，(.exe格式)，然后双击打开，看是否能够使用.						
				
				
1、打开文件						
				
2、得到文件的大小						
				
3、申请内存						
				
4、把文件中的内容读取到内存中 						
				
5、返回内存编号	


1、fopen函数

2、fseek函数

3、ftell函数

4、fclose函数

5、fread函数


*/


// 1、用十六进制文本编辑器，打开一个记事本的.exe文件，再打开在内存中的记事本						
// 进程，记录下这两个文件的不同.
// 前面的部分都是相同的  后面的部分内存中的记事本空白部分更多  数据被拉伸了


int main(int argc, char* argv[])
{
	Data* FileSizeResult = GetFileSize("D:\\study\\C_X86\\HD\\汇编\\6(标志寄存器)\\2015-01-19\\作业\\NEW_CRACKME.EXE");  
	// 打开指定的文件流 FileSizeResult 为获取的文件大小

	char* pMemory = RequestMemory(FileSizeResult -> SizeOfFile);  // 根据所需空间申请内存

	char* pMemoryAddress = ReadFileToMemory(pMemory, sizeof(char), \
		FileSizeResult -> SizeOfFile, FileSizeResult -> FileStream);  // 把文件中的内容读取到内存中并返回内存编号

	if (pMemoryAddress == NULL)
	{
		printf("文件读取失败\n");
	}
	else
	{
		printf("文件读取成功,所在内存地址的编号是: %x\n", pMemoryAddress);
	}
	
	CloseFileStream(FileSizeResult -> FileStream);  // 关闭读文件流管道

	// ------------------ BEGIN 可以在这里修改PE文件的数据------------------

	// PrintFormatOfPe(FileSizeResult -> SizeOfFile, pMemoryAddress);  // 打印在堆中的PE文件数据


	// ------------------ END 可以在这里修改PE文件的数据------------------
	/* 
	int result = WriteMemoryToNewFile(pMemoryAddress, sizeof(char), FileSizeResult->SizeOfFile, \
		"C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\new.exe");  // 将流数据写入到文件
	
	if (result)
	{
		printf("数据流写入文件错误\n");
	}
	else
	{
		printf("数据流写入文件成功\n");
	}

	CloseFileStream(FileSizeResult->WriteFileStream);  // 关闭写文件流管道
	*/

	getchar(); // 未释放内存空间的时候,此时PE文件内存未清理

	FreeMemory(pMemory);  // 释放堆空间 初始化堆指针

	pMemoryAddress = NULL;  // 初始化指针的值

	getchar();

	return 0;
}
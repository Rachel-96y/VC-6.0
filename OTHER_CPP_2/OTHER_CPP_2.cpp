// OTHER_CPP_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
#include <iostream>

/* 需要更改 2 个路径 + 一个 MessageBox 地址才能正常运行 */

// ************ 更改 ************
// LPSTR lpszFile = (LPSTR)("D:\\\\BaiduNetdiskDownload\\\\关键词.exe"); 
#define lpszFile "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\notepad.exe"

// ************ 更改 ************
// WORD Maddress = 0x761328B0; 
DWORD Maddress = 0x76691610;

// ************ 更改 ************
/*		
BYTE ShellCode[] = {	
	0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00 ,
	0xE8, 0x00, 0x00, 0x00, 0x00,
	0xE9, 0x00, 0x00, 0x00,0x00,0x00	
};	
*/

BYTE ShellCode[] = {
	0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00 ,
	0xE8, 0x00, 0x00, 0x00, 0x00,
	0xE9, 0x00, 0x00, 0x00, 0x00
};



int main(int argc, char* argv[])
{
	DWORD Codesize = sizeof(ShellCode);
	printf("%d", Codesize);
	// printf("%s\n", lpszFile);
	PVOID pFileBuffer = 0;
	PVOID pImageBuffer = 0;
	PVOID pNewBuffer = 0;
	DWORD FileBuffersize = CreateFileBuffer(lpszFile,&pFileBuffer);
	// 打印PE头文件
	ToLoaderPE(pFileBuffer);
	
	// DWORD ImageBuffersize = CopyFileBufferToImageBuffer(pFileBuffer, &pImageBuffer);
	// printf("Buffer长度:%x\nBuffer指针:%x\n", FileBuffersize, pFileBuffer);
	// printf("ImageBuffer长度:%x\nImageBuffer指针:%x\n", ImageBuffersize, pImageBuffer);

	// DWORD pNewBuffersize = ImageBufferToNewBuffer(pImageBuffer, &pNewBuffer);
	// printf("pNewBuffersize长度:%x\npNewBuffer指针:%x\n", pNewBuffersize, pNewBuffer);
	
	// RVA  FOA  VA转换
	// convertFoaToRva(IN lpszFile);
	
	// 节内空白区添加代码
	ChangeImageBufferCode1(IN lpszFile, IN ShellCode, IN  Codesize, IN Maddress);

	// 文件存盘  
	// WriteMemeryToFile(pNewBuffer, pNewBuffersize);
	
	// std::cout << "Hello World!\n";

	// ************ 添加 ************
	system("pause");	
	
	return 0;
}

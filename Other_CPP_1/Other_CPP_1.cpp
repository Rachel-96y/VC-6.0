// Other_CPP_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// 目标 EXE 程序路径
LPSTR lpszFile = (LPSTR)("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\notepad.exe");

// OUT
#define PATH "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\NEW_notepad.exe"

// MessageBoxA的地址
DWORD Maddress = 0x76691610;

// 将要注入的代码
UCHAR ShellCode[] = {
	0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00 ,
	0xE8, 0x00, 0x00, 0x00, 0x00,
	0xE9, 0x00, 0x00, 0x00, 0x00
};


int main()
{
	// 获取要注入的代码大小
	DWORD Codesize = sizeof(ShellCode);

	// 节内空白区添加代码
	ChangeImageBufferCode1(lpszFile, ShellCode, Codesize, Maddress);

	getchar();

	return 0;
}



	/*
	// 3个 Buffer 的指针
	// LPVOID pFileBuffer = NULL;

	// LPVOID pImageBuffer = NULL;

	// LPVOID pNewBuffer = NULL;

	// 得到 FileBuffer 的大小 
	// DWORD FileBuffersize = CreateFileBuffer(lpszFile, &pFileBuffer);

	// 申请 ImageBuffer 更改 pImageBuffer 指向 得到 ImageBuffersize 大小
	// DWORD ImageBuffersize = CopyFileBufferToImageBuffer(pFileBuffer, &pImageBuffer);

	// DWORD SizeOfNewBuffer = ImageBufferToNewBuffer(pImageBuffer, &pNewBuffer);

	
	// 文件存盘  
	// WriteMemeryToFile(pNewBuffer, SizeOfNewBuffer);
	*/
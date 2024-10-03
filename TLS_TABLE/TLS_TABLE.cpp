// TLS_TABLE.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

DWORD PrintTLS(LPVOID lpFileBuffer)
{
	// 打印32位的TLS表

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 首先判断PE文件是否有TLS表
	if (!(pIMAGE_DATA_DIRECTORY + 9) -> VirtualAddress)
	{
		printf("此PE文件没有TLS表");
		return RETURN_FAILURE;
	}

	// 打印TLS表RVA及大小
	printf("TLS表所在位置RVA:%x\nTLS表大小:%x\n", (pIMAGE_DATA_DIRECTORY + 9) -> VirtualAddress, (pIMAGE_DATA_DIRECTORY + 9) -> Size);

	// 定义TLS表结构体指针
	PIMAGE_TLS_DIRECTORY32 pIMAGE_TLS_DIRECTORY32 = (IMAGE_TLS_DIRECTORY32*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 9) -> VirtualAddress, lpFileBuffer) + (DWORD)lpFileBuffer);
	
	// TLS表数据
	printf("初始化数据的起始地址:%x\n", pIMAGE_TLS_DIRECTORY32 -> StartAddressOfRawData);
	printf("初始化数据的结束地址:%x\n", pIMAGE_TLS_DIRECTORY32 -> EndAddressOfRawData);
	printf("索引位置:%x\n", pIMAGE_TLS_DIRECTORY32 -> AddressOfIndex);
	printf("回调函数数组指针:%x\n", pIMAGE_TLS_DIRECTORY32 -> AddressOfCallBacks);
	printf("填充0的个数:%x\n", pIMAGE_TLS_DIRECTORY32 -> SizeOfZeroFill);
	printf("保留字段:%x\n", pIMAGE_TLS_DIRECTORY32 -> Characteristics);

	DWORD dwNumber = 0;
	printf("函数个数不一定准确\n");
	// TLS表回调函数地址
	do
	{
		printf("函数地址存储位置: %x\n", pIMAGE_TLS_DIRECTORY32 -> AddressOfCallBacks + dwNumber);
	}
	while(*(pIMAGE_TLS_DIRECTORY32 -> AddressOfCallBacks + ++dwNumber));

	return RETURN_SUCCESS;
}

int main(int argc, char* argv[])
{
	// 打印TLS表 (数据目录中第10个[下标为9])
	LPVOID lpFileBuffer = NULL;
	IsPeFile(PATH, &lpFileBuffer);
	PrintTLS(lpFileBuffer);

	system("pause");
	return 0;
}
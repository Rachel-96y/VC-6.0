// A_TEST_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

VOID PRINT_BASE_RELOCATION(IN LPVOID pFileBuffer)
{
	/* 打印所有重定位表及需要修改的值的信息 */

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pPIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 计算出重定位表的 FOA 并加上 pFileBuffer 的值 赋值给重定位表结构体指针
	PIMAGE_BASE_RELOCATION pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa((pPIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);
	
	// 开始打印重定位表的数据
	while((pPIMAGE_BASE_RELOCATION + 1) -> SizeOfBlock && (pPIMAGE_BASE_RELOCATION + 1) -> VirtualAddress)
	{
		// 如果 VirtualAddress 或者 SizeOfBlock 的值为0 则重定位表结束

		// SizeOfBlock 块大小 
		printf("RVA:%x\t块大小:%x\n", pPIMAGE_BASE_RELOCATION -> VirtualAddress, pPIMAGE_BASE_RELOCATION -> SizeOfBlock);

		// items 项目数
		printf("项目数: %xH\n", (pPIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / 2);

		printf("块数据: (需要校准的地址的偏移)\n");
		// 打印块内的 需要校准的地址的值 (用于对齐的值则忽略)  这里不需要打印2个字节 只需要 12 位即可
		for (UINT i = 0; i < (pPIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / 2; i++)
		{
			// 可以判断前 4 位是否是 0011 如果符合则打印其偏移
			// if (*(LPWORD)((DWORD)(pPIMAGE_BASE_RELOCATION) + IMAGE_SIZEOF_BASE_RELOCATION + i * sizeof(USHORT)) & 0x3000 == 0x3000);

			// 将需要打印的值去掉高 4 位后打印出来其 RVA 和 FOA
			USHORT TEMP_RVA = *(LPWORD)((DWORD)(pPIMAGE_BASE_RELOCATION) + IMAGE_SIZEOF_BASE_RELOCATION + i * sizeof(USHORT));

			// 将高 4 位置零
			TEMP_RVA &= 0x0FFF;
			// 无符号数如果使用 >> 相当于 SHR 而非 SAR 
			// TEMP_RVA <<= 4;
			// TEMP_RVA >>= 4;

			// 表中的偏移量加上 VirtualAddress 等于真正的 VirtualAddress
			printf("Rva : %x\t", TEMP_RVA + pPIMAGE_BASE_RELOCATION -> VirtualAddress);
			printf("Foa : %x\t", ChangeRvaToFoa(TEMP_RVA + pPIMAGE_BASE_RELOCATION -> VirtualAddress, pFileBuffer));

			// 这里的 Address 可能不是这样计算的
			printf("Address : %x\n", ChangeRvaToFoa(TEMP_RVA + pPIMAGE_BASE_RELOCATION -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);

		}
		printf("--------------------------------------------------------------------------------------\n");

		// 跳过一个块的全部地址 注意这里不需要加 IMAGE_SIZEOF_BASE_RELOCATION
		pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pPIMAGE_BASE_RELOCATION + pPIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}

	return;
}

int main(int argc, char* argv[])
{
	/*
		本节练习：

		1、打印所有重定位信息

		2、重定位表这样设计有什么好处
	*/

	// 重定位表这样设计的好处是节省内存

	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将 PE 文件读取进 FileBuffer 中
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 打印所有重定位表及需要修改的值的信息
	PRINT_BASE_RELOCATION(pFileBuffer);

	getchar();
	
	return 0;
}
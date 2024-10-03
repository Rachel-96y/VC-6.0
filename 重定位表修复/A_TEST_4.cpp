// A_TEST_4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


VOID TestDll()
{
	/* 此函数用于测试被修改后的DLL能不能正常执行 */

	HMODULE hmodule = LoadLibrary(NEW_PATH);

	int (__stdcall *pPlus) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Plus@8");
	int (__stdcall *pSub) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Sub@8");
	int (__stdcall *pMul) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Mul@8");
	int (__stdcall *pDiv) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Div@8");

	int ret1 = pPlus(4, 3);
	int ret2 = pSub(7, 2);
	int ret3 = pMul(6, 4);
	int ret4 = pDiv(9, 3);

	printf("%d\n%d\n%d\n%d", ret1, ret2, ret3, ret4);
	
	return;
}


VOID ChangeImageBase(IN LPVOID pNewFileBuffer)
{
	/* 修改DLL的ImageBase,根据重定位表修正 */
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pNewFileBuffer;

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
	PIMAGE_BASE_RELOCATION pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa((pPIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, pNewFileBuffer) + (DWORD)pNewFileBuffer);

	
	// 更改 ImageBase 的值 偏移为 64K 的倍数
	pPIMAGE_OPTIONAL_HEADER32 -> ImageBase += 0x10000;

	// 判断是否第一个块有值
	if (!(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress))
	{
		// 如果重定位表没有值则 直接返回
		return;
	}

	do
	{
		// 因为 ImageBase 的值加了 0x10000 故将重定位表中的地址指向的地址的值都需要加 0x10000 (64K 的倍数)

		// 每个块可能需要校准的第一个地址
		LPWORD BASE_RELOCATION_PTR = (LPWORD)((DWORD)pPIMAGE_BASE_RELOCATION + IMAGE_SIZEOF_BASE_RELOCATION);

		// 除结构体的两个成员外 计算有多少项目
		DWORD NumberOfBaseRelocation = (pPIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / 2;

		for (UINT i = 0; i < NumberOfBaseRelocation; i++)
		{
			// 如果此项有意义(前 4 位等于0011)
			if ((*BASE_RELOCATION_PTR & 0x3000) == 0x3000)
			{
				// 将值取出来进行计算
				UINT TEMP_NUMBER = *BASE_RELOCATION_PTR;
				
				// 将高 4 位置零
				TEMP_NUMBER &= 0x0FFF;

				// 找到需要修改的地址 需要转为FOA
				LPDWORD RelocationNumber = (LPDWORD)(ChangeRvaToFoa(TEMP_NUMBER + pPIMAGE_BASE_RELOCATION -> VirtualAddress, pNewFileBuffer) + (DWORD) pNewFileBuffer);
				*RelocationNumber += 0x10000;
			}

			BASE_RELOCATION_PTR++;
		}
		
		// 跳过一个块的全部地址 注意这里不需要加 IMAGE_SIZEOF_BASE_RELOCATION
		pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pPIMAGE_BASE_RELOCATION + pPIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}	
	// 如果 VirtualAddress 或者 SizeOfBlock 的值为0 则重定位表结束
	while(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress);


	return;
}



int main(int argc, char const *argv[])
{
	/* 修改DLL的ImageBase,根据重定位表修正，然后存盘.看DLL是否可以使用. */



	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD SIZE_OF_FILE_BUFFER = ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 修改DLL的ImageBase,根据重定位表修正
	ChangeImageBase(pFileBuffer);

	// 存盘
	WriteMemeryToFile(pFileBuffer, SIZE_OF_FILE_BUFFER, NEW_PATH);

	// ----------------------------------------------------
	// 测试DLL是否可以正常使用
	TestDll();  // NEW_PATH
	// ----------------------------------------------------
	
	getchar();

	return 0;
}

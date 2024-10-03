// A_TEST_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

BOOL PrintExport(LPVOID pFileBuffer)
{
	/* 打印所有的导出表信息 */

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

	// 计算出在 FileBuffer 中导出表的实际偏移地址(FOA) 加上 FileBuffer 的基地址赋值给导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);

	// 打印数据目录中导出表的值
	printf("数据目录(导出表):\nVirtualAddress:		%x\n", pPIMAGE_DATA_DIRECTORY -> VirtualAddress);
	printf("Size:			%x\n\n", pPIMAGE_DATA_DIRECTORY -> Size);

	// 打印出所有的导出表信息
	printf("导出表信息:\nCharacteristics:	%d\n", pPIMAGE_EXPORT_DIRECTORY -> Characteristics);	// 已弃用
	printf("TimeDateStamp:		%d\n", pPIMAGE_EXPORT_DIRECTORY -> TimeDateStamp);
	printf("MajorVersion:		%d\n", pPIMAGE_EXPORT_DIRECTORY -> MajorVersion);		// 已弃用
	printf("MinorVersion:		%d\n", pPIMAGE_EXPORT_DIRECTORY -> MinorVersion);		// 已弃用
	printf("Name:			%d\n", pPIMAGE_EXPORT_DIRECTORY -> Name);
	printf("Base:			%d\n", pPIMAGE_EXPORT_DIRECTORY -> Base);		// Base + 每一个序号表的值 = 导出时的序号表
	printf("NumberOfFunctions:	%d\n", pPIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions);	// 导出函数的个数
	printf("NumberOfNames:		%d\n", pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames);		// 以函数名导出的函数的个数

	printf("AddressOfFunctions:	%x\n", pPIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions);		// 导出函数地址表地址RVA
	printf("AddressOfNames:		%x\n", pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames);		// 导出函数名称表地址RVA
	printf("AddressOfNameOrdinals:	%x\n", pPIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals);		// 导出函数序号表地址RVA
	// 导出表中存储函数地址的地址
	LPDWORD PointerToFunctions = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions, pFileBuffer) + (DWORD)pFileBuffer);
	
	// 打印出所有函数的地址
	for (UINT i = 0; i< pPIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions; i++)
	{
		printf("函数地址(RVA偏移): %x\n", *(PointerToFunctions + i));
	}

	// 导出表中存储函数名字地址的地址
	LPDWORD PointerToNames = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames, pFileBuffer) + (DWORD)pFileBuffer);
	for (i = 0; i < pPIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions; i++)
	{
		// 注意 AddressOfNames 中的地址是偏移地址 需要加上 pFileBuffer
		// 注意 以下↓打印 不能作用于具有 NONAME 的模块
		printf("函数名字: %s\n", *(PointerToNames + i) + (DWORD)pFileBuffer);
	}

	// 导出表中存储函数序列的地址  (序列+base = 导出序列) 
	LPWORD PointerToNameOrdinals = (LPWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals, pFileBuffer) + (DWORD)pFileBuffer);
	for (i = 0; i < pPIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions; i++)
	{
		printf("函数序号: %x\n", *(PointerToNameOrdinals + i));
	}
	return 0;
}

int main(int argc, char* argv[])
{
	/*
		练习：

		1、编写一个 RVA 转 FOA 的函数

		2、编写程序打印所有的导出表信息

		3、GetFunctionAddrByName(FileBuffer指针，函数名指针)

		4、GetFunctionAddrByOrdinals(FileBuffer指针，函数名导出序号)
	*/

	

	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将 PE 文件读取进 FileBuffer 中
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 打印所有的导出表的信息
	PrintExport(pFileBuffer);	

	// 根据指定函数名字找到函数在模块中的偏移地址
	LPVOID FunctionAddr = GetFunctionAddrByName(pFileBuffer, "Sub"); 
	printf("%x\n", FunctionAddr);

	// 根据指定序号找到函数在模块中的偏移地址
	LPVOID FunctionAddr2 = GetFunctionAddrByOrdinals(pFileBuffer, 13);
	printf("%x\n", FunctionAddr2);

	getchar();

	return 0;
}
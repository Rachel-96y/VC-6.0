// A_TEST_6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

BOOL PRINT_IMAGE_DATA_DIRECTORY(IN LPVOID pFileBuffer)
{
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

	// 判断DOS_MZ头是否有效
	if (pPIMAGE_DOS_HEADER -> e_magic != IMAGE_DOS_SIGNATURE)
	{
		printf("这不是一个有效的PE结构: 错误的MZ头标志");
		return NULL;
	}

	// 判断PE标识是否有效
	if (*(PDWORD)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew) != IMAGE_NT_SIGNATURE)
	{
		printf("这不是一个有效的PE结构: 错误的PE标志");
		return NULL;
	}

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pPIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	
	// 定义导入表结构体指针
	PIMAGE_IMPORT_DESCRIPTOR pPIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa((pPIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);
	
	// 定义绑定导入表结构体指针
	PIMAGE_BOUND_IMPORT_DESCRIPTOR pPIMAGE_BOUND_IMPORT_DESCRIPTOR = (IMAGE_BOUND_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa((pPIMAGE_DATA_DIRECTORY + 11) -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);
	
	// 绑定导入表中的第一个结构的 DESCRIPTOR 加上 OffsetModuleName 可以得到模块名
	DWORD dwNameBase = (DWORD)pPIMAGE_BOUND_IMPORT_DESCRIPTOR;

	// 打印第一个绑定导入表中的其它值
	printf("FirstTimeDateStamp: %x\n", pPIMAGE_BOUND_IMPORT_DESCRIPTOR -> TimeDateStamp);
	printf("FirstModuleName: %s\n", pPIMAGE_BOUND_IMPORT_DESCRIPTOR -> OffsetModuleName + dwNameBase);
	// 打印第一个绑定导入表中 NumberOfModuleForwarderRefs 里记录的剩余表数量 这个值可能不准(可能为0)
	printf("FirstNumberOfModuleForwarderRefs: %x\n", pPIMAGE_BOUND_IMPORT_DESCRIPTOR -> NumberOfModuleForwarderRefs);
	printf("\n");

	// 定义绑定导入表的 _IMAGE_BOUND_FORWARDER_REF 结构
	PIMAGE_BOUND_FORWARDER_REF pPIMAGE_BOUND_FORWARDER_REF = (IMAGE_BOUND_FORWARDER_REF*)++pPIMAGE_BOUND_IMPORT_DESCRIPTOR;

	do
	{
		// 如果有一个这样的结构体的值全为0则跳出
		if (!(pPIMAGE_BOUND_FORWARDER_REF ->TimeDateStamp  | \
			pPIMAGE_BOUND_FORWARDER_REF -> OffsetModuleName | \
			pPIMAGE_BOUND_FORWARDER_REF -> Reserved))
			break;

		// 判断是否此模块有绑定导入表
		if (!pPIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp)
		{
			// 如果此模块没有绑定导入表则跳过
			pPIMAGE_IMPORT_DESCRIPTOR++;

			// 转到下一个绑定导入表
			pPIMAGE_BOUND_FORWARDER_REF++;

			// 没用绑定导入表就跳转至下一个
			continue;
		}

		// 打印绑定导入表
		printf("TimeDateStamp: %x\n", pPIMAGE_BOUND_FORWARDER_REF -> TimeDateStamp);
		printf("ModuleName: %s\n", pPIMAGE_BOUND_FORWARDER_REF -> OffsetModuleName + dwNameBase);
		printf("Reserved: %x\n", pPIMAGE_BOUND_FORWARDER_REF -> Reserved);
		printf("\n");

		// 转到下一个绑定导入表
		pPIMAGE_BOUND_FORWARDER_REF++;

		// 转到下一个导入表
		pPIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);

	return NULL;
}

int main(int argc, char* argv[])
{
	/*
		练习：
		打印绑定导入表
	*/

	// PATH NEW_PATH 
	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(NEW_PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD SIZE_OF_FILE_BUFFER = ReadPeFileToFileBuffer(NEW_PATH, pFileBuffer);

	// 打印绑定导入表
	PRINT_IMAGE_DATA_DIRECTORY(pFileBuffer);

	getchar();

	return 0;
}


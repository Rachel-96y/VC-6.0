// stdafx.cpp : source file that includes just the standard includes
//	project_1.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

// ------以下为函数实现------


// ************************************************************************

LPVOID CreateFileBuffer(IN LPSTR lpszFile)
{
	/* 
		根据给定的文件路径读取硬盘上的文件
		获取文件大小(字节) 
		根据文件大小申请一块相同大小的内存空间
		由此申请的空间不在此函数进行释放
		如果执行成功则返回已经申请的空间的首地址指针 否则返回0 
	*/

	// 根据文件路径(char*)打开此文件的管道 返回值是指向文件结构的指针
	FILE* pOpenFile = fopen(lpszFile, "rb");
	if (pOpenFile == NULL)
	{
		// 返回空指针表明文件打开出现错误
		printf("文件打开出现错误\n");
		return NULL;
	}

	// 调整文件指针至文件末尾
	fseek(pOpenFile, 0, SEEK_END);

	// 获取文件大小
	DWORD SizeOfFile = ftell(pOpenFile);

	if (SizeOfFile == -1L)
	{
		// ftell 返回值为-1L代表此函数执行出现错误
		printf(" ftell 函数执行失败\n");
		return NULL;
	}

	// 调整文件指针至文件开头
	fseek(pOpenFile, 0, SEEK_SET);

	// 关闭文件管道
	DWORD FcloseReturnValue = fclose(pOpenFile);

	if (FcloseReturnValue)
	{
		printf(" 文件管道关闭失败\n");
		return NULL;
	}

	// 为此文件申请一块相同大小的空间
	LPVOID pFileBuffer = malloc(sizeof(CHAR) * (UINT)SizeOfFile); 
	if (pFileBuffer == NULL)
	{
		printf("空间申请失败\n");
		return NULL;
	}
	// 将申请的空间内的数据初始化为 0
	memset(pFileBuffer, 0x00000000, sizeof(CHAR) * (UINT)SizeOfFile);

	return pFileBuffer;
}

// ************************************************************************

DWORD MyAlignment(IN LPVOID pPeMemory, IN UINT FileOrSectionAlignment,IN UINT SizeOfCode)
{
	/*
		根据提供的 pPeMemory 及 需要对齐的字节数 及 对齐方式来计算出对齐后的结果
		如果执行成功则返回对齐后的结果 失败则返回0
	*/
	
	// 获取 pPeMemory 所指向的内存的PE文件的两种对齐参数

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pPeMemory;

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
		printf("这不是一个有效的PE文件: 错误的MZ头标志");
		return NULL;
	}
	// 判断PE标识是否有效
	if (*(PDWORD)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew) != IMAGE_NT_SIGNATURE)
	{
		printf("这不是一个有效的PE文件: 错误的PE标志");
		return NULL;
	}

	// 根据对齐参数及对齐方式进行计算并返回结果
	DWORD AlignmentNumber = 0x00000000;

	if (!FileOrSectionAlignment)
	{
		// 以文件对齐的参数对齐 FileOrSectionAlignment == 0
		if (!(SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> FileAlignment))
		{
			// 如果内存对齐的大小 大于要写入的代码的大小 则要添加的代码大小根据内存对齐的大小来算
			AlignmentNumber = pPIMAGE_OPTIONAL_HEADER32 -> FileAlignment;
		}
		if (SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> FileAlignment)
		{
			// 如果内存对齐的大小 小于要写入的代码的大小 则需要进行计算  
			if (!(SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> FileAlignment))
			{
				// 如果刚好是内存对齐的整数倍 则要写入的代码的大小就等于此大小
				AlignmentNumber = SizeOfCode;
			}
			if (SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> FileAlignment)
			{
				// 如果输入的代码长度 大于内存对齐的值 并且不是内存对齐的整数倍 
				// 则要写入的代码的大小 等于输入的代码大小除以内存对齐的值的结果 + 1 再乘以内存对齐的参数
				AlignmentNumber =  ((SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> FileAlignment) + 1) * pPIMAGE_OPTIONAL_HEADER32 -> FileAlignment;
			}
		}

		return AlignmentNumber;
	}
	else
	{
		// 以内存对齐的参数对齐 FileOrSectionAlignment != 0
		if (!(SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
		{
			// 如果内存对齐的大小 大于要写入的代码的大小 则要添加的代码大小根据内存对齐的大小来算
			AlignmentNumber = pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
		}
		if (SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
		{
			// 如果内存对齐的大小 小于要写入的代码的大小 则需要进行计算  
			if (!(SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
			{
				// 如果刚好是内存对齐的整数倍 则要写入的代码的大小就等于此大小
				AlignmentNumber = SizeOfCode;
			}
			if (SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
			{
				// 如果输入的代码长度 大于内存对齐的值 并且不是内存对齐的整数倍 
				// 则要写入的代码的大小 等于输入的代码大小除以内存对齐的值的结果 + 1 再乘以内存对齐的参数
				AlignmentNumber =  ((SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment) + 1) * pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
				// 此时的 SizeOfCodeSectionAlignment 为真正要添加的节的数据的大小
		
			}
		}

		return AlignmentNumber;
	}

}

// ************************************************************************

DWORD ReadPeFileToFileBuffer(IN LPSTR pPeFilePath, OUT LPVOID pFileBuffer)
{
	/* 
		根据给定的文件路径读取硬盘上的文件
		将其读取进指定的内存空间
		如果函数执行成功则返回读取的文件的大小(字节) 失败返回0
	*/

	FILE* pOpenFile = fopen(pPeFilePath, "rb");
	if (pOpenFile == NULL)
	{
		// 返回空指针表明文件打开出现错误
		printf("文件打开失败\n");
		return NULL;
	}

	// 调整文件指针至文件末尾
	fseek(pOpenFile, 0, SEEK_END);

	// 获取文件大小
	DWORD SizeOfFile = ftell(pOpenFile);
	if (SizeOfFile == -1L)
	{
		// ftell 返回值为-1L代表此函数执行出现错误
		printf(" ftell 函数执行失败\n");
		return NULL;
	}
	// 调整文件指针至文件开头
	fseek(pOpenFile, 0, SEEK_SET);

	DWORD SizeOFReadFile = fread(OUT pFileBuffer, sizeof(CHAR), sizeof(CHAR) * SizeOfFile, IN pOpenFile);
	
	// 关闭文件管道
	DWORD FileCloseReturnValue = fclose(pOpenFile);
	if (FileCloseReturnValue)
	{
		printf("文件管道关闭失败\n");
		return NULL;
	}

	return SizeOFReadFile;
}

// ************************************************************************

DWORD WriteMemeryToFile(IN LPVOID pMemBuffer, IN UINT SizeOfFile, OUT LPSTR lpszFile)
{
	/*
		根据给定的 pMemBuffer 及文件路径和所要写入的文件大小
		将内存中的数据写入到磁盘上
		如果存盘成功则返回写入数据的大小 失败则返回0
	*/
	FILE* pOpenFile = fopen(lpszFile, "wb");
	if (pOpenFile == NULL)
	{
		// 返回空指针表明文件打开出现错误
		printf("文件打开失败");
		return NULL;
	}

	// 调整文件指针至文件开头
	fseek(pOpenFile, 0, SEEK_SET);

	// 将内存中的数据写入到指定硬盘路径
	DWORD SizeOfWriteDate = fwrite(pMemBuffer, sizeof(CHAR), sizeof(CHAR) * SizeOfFile, pOpenFile);
	if (SizeOfWriteDate != SizeOfFile)
	{
		printf("写入失败");
		return NULL;
	}

	// 关闭文件管道
	DWORD FileCloseReturnValue = fclose(pOpenFile);
	if (FileCloseReturnValue)
	{
		printf("文件管道关闭失败\n");
		return NULL;
	}

	return SizeOfWriteDate;
}

// ************************************************************************

DWORD ChangeRvaToFoa(IN UINT RVA, IN LPVOID pFileBuffer)
{
	/*
		此函数接收一个 RVA 及一个 FileBuffer 指针 返回一个 FOA
		如果执行成功则返回该值在文件中的偏移 FOA 失败则返回0
	*/

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

	// 声明一个 Foa 用于存储最终的结果
	DWORD FOA = 0;

	// 计算 Rva 与 pImageBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0; 

	// 判断输入进来的 Rva 将落在 ImageBuffer 中的哪一个节里
	if (RVA < pPIMAGE_SECTION_HEADER -> VirtualAddress)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		FOA = RVA;
		return FOA;
	}

	for (int i = 0; i < (pPIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (RVA >= (pPIMAGE_SECTION_HEADER + pPIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress)
		{
			// 计算 Rva 与最后一个节的 VirtualAddress 之间的偏移
			TempNumber = RVA - (pPIMAGE_SECTION_HEADER + pPIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress;

			// Foa 的值为 最后一个节的 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pPIMAGE_SECTION_HEADER + pPIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData + TempNumber;
		}

		// 如果落在除最后一个节外的其它节里
		if (RVA >= (pPIMAGE_SECTION_HEADER + i) -> VirtualAddress && RVA < (pPIMAGE_SECTION_HEADER + i + 1) -> VirtualAddress)
		{
			// 计算 Rva 与 VirtualAddress 之间的偏移
			TempNumber = RVA - (pPIMAGE_SECTION_HEADER + i) -> VirtualAddress;

			// Foa 的值为 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pPIMAGE_SECTION_HEADER + i) -> PointerToRawData + TempNumber;
			
		}
	}

	return FOA;
}

// ************************************************************************

DWORD ChangeFoaToRva(IN UINT FOA, IN LPVOID pFileBuffer)
{
	/*
		此函数接收一个 FOA 及一个 FileBuffer 指针 返回一个 RVA
		如果执行成功则返回该值在文件中的偏移 RVA 失败则返回0
	*/

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

	// 声明一个 Rva 用于存储最终的结果
	DWORD RVA = 0;

	// 计算 Foa 与 pFileBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0; 

	// 判断输入进来的 Foa 将落在 FileBuffer 中的哪一个节里
	if (FOA < pPIMAGE_SECTION_HEADER -> PointerToRawData)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		RVA = FOA;
		return RVA;
	}

	for (int i = 0; i < (pPIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (FOA >= (pPIMAGE_SECTION_HEADER + pPIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData)
		{
			// 计算 Foa 与最后一个节的 PointerToRawData 之间的偏移
			TempNumber = FOA - (pPIMAGE_SECTION_HEADER + pPIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData;

			// RVA 的值为 最后一个节的 VirtualAddress + TempNumber
			RVA = (pPIMAGE_SECTION_HEADER + pPIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress + TempNumber;
		}

		// 如果落在除最后一个节外的其它节里
		if (FOA >= (pPIMAGE_SECTION_HEADER + i) -> PointerToRawData && FOA < (pPIMAGE_SECTION_HEADER + i + 1) -> PointerToRawData)
		{
			// 计算 FOA 与 PointerToRawData 之间的偏移
			TempNumber = FOA - (pPIMAGE_SECTION_HEADER + i) -> PointerToRawData;

			// RVA 的值为 VirtualAddress + TempNumber
			RVA = (pPIMAGE_SECTION_HEADER + i) -> VirtualAddress + TempNumber;
			
		}
	}


	return RVA;
}

// ************************************************************************

LPVOID GetFunctionAddrByName(IN LPVOID pFileBuffer, IN LPSTR FunctionName)
{
	/*
		根据名字获取函数在模块中的地址并返回
		函数执行成功则返回所找函数的地址 失败则返回0
	*/

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

	// 计算出在 FileBuffer 中导出表的实际偏移地址(FOA) 加上 FileBuffer 的基地址赋值给导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);

	// 找到导出表的 AddressOfNames 的起始地址 
	LPDWORD PointerToNames = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames, pFileBuffer) + (DWORD)pFileBuffer);

	for (UINT i = 0; i < pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames; i++)
	{
		// 遍历 NumberOfNames 表, 找到和指定函数名称相同的值
		if (strcmp(FunctionName + i, (LPSTR)(*(PointerToNames + i) + (DWORD)pFileBuffer)) == 0)
		{
			//如果根据名字找到了是哪一个函数 则再根据序号去序号表里找
			// 找到 AddressOfNameOrdinals 的起始地址
			LPWORD PointerToNameOrdinals = (LPWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals, pFileBuffer) + (DWORD)pFileBuffer);

			// 以 i 的值为下标 获取到对应的序号表的值
			WORD SubScript = (WORD)*(PointerToNameOrdinals + i);

			// 找到 AddressOfFunctions 的起始地址
			LPDWORD PointerToFunctions = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions, pFileBuffer) + (DWORD)pFileBuffer);

			// 以获取到的序号表里的值 为下标 获取到对应的函数的地址
			LPVOID FunctionAddr = (LPVOID)*(PointerToFunctions + SubScript);

			return FunctionAddr;
		}
	}
	
	return NULL;
}

// ************************************************************************

LPVOID GetFunctionAddrByOrdinals(IN LPVOID pFileBuffer, IN UINT FunctionOrdinal)
{
	/*
		根据序号获取函数在模块中的地址并返回
		函数执行成功则返回所找函数的地址 失败则返回0
	*/

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

	// 计算出在 FileBuffer 中导出表的实际偏移地址(FOA) 加上 FileBuffer 的基地址赋值给导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);

	// 找到导出表的 AddressOfFunctions 的起始地址 
	LPDWORD* PointerToFunctions = (LPDWORD*)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions, pFileBuffer) + (DWORD)pFileBuffer);

	// 根据所给的序号 找出函数的地址
	LPVOID FunctionAddr = (LPVOID)*(PointerToFunctions + (FunctionOrdinal - pPIMAGE_EXPORT_DIRECTORY -> Base));

	return FunctionAddr;
}

// ************************************************************************

DWORD SizeOfDllFunctionLength(IN LPSTR DLL_PATH)
{
	/*
		根据给定的 DLL 路径获取其中的第一个函数名的长度
	*/

	// 根据文件大小申请一块内存
	LPVOID pFileBuffer_DLL = CreateFileBuffer(DLL_PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD SizeOf_ReadPeFileToFileBuffer = ReadPeFileToFileBuffer(DLL_PATH, pFileBuffer_DLL);
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer_DLL;

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

	// 如果不是 DLL 文件则返回 NULL 
	if (!(pPIMAGE_FILE_HEADER -> Characteristics & 0x2000))
	{
		printf("这不是一个有效的 DLL 文件");
		return NULL;
	}

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pPIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	
	// 定义导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pFileBuffer_DLL) + (DWORD)pFileBuffer_DLL);
	
	// 判断此 DLL 是否有导出的函数 没有则返回 NULL
	if (!pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames)
	{
		// 目前仅支持以名字导出的函数的 DLL 进行注入
		printf("此 DLL 没有以名字导出的函数");
		return NULL;
	}
		
	// 如果有以名字导出的函数则找到函数名地址的地址
	LPDWORD dwFunctionNameAddr = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames, pFileBuffer_DLL) + (DWORD)pFileBuffer_DLL);
	
	// 得到函数名的地址的 FOA 计算出导出函数的函数名长度
	DWORD dwFunctionNameLength = strlen((LPSTR)(ChangeRvaToFoa(*dwFunctionNameAddr, pFileBuffer_DLL) + (DWORD)pFileBuffer_DLL));

	return dwFunctionNameLength;
}

// ************************************************************************

LPVOID CleanPeData(IN LPVOID pFileBuffer, IN UINT SizeOfFileBuffer)
{
	/*
		如果文件最后存在不加载到内存中的数据则重新创建一个 NewFileBuffer
		将 FileBuffer 中需要加载到内存中的数据拷贝过去 并释放 FileBuffer 空间
	*/

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

	// 定位到最后一个节表
	pPIMAGE_SECTION_HEADER += (pPIMAGE_FILE_HEADER -> NumberOfSections - 1);
	
	// 得到这个节表的最后的位置 = PointerToRawDate + SizeOfRawData (即: FileBuffer 真正的大小)
	DWORD TrueSizeOfFileBuffer = pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> SizeOfRawData;

	// 判断 SizeOfFileBuffer 和 TrueSizeOfFileBuffer 大小是否相等 
	if (SizeOfFileBuffer == TrueSizeOfFileBuffer) 
	{

		// 如果相等则返回 pFileBuffer 指针
		return pFileBuffer;
	}
	else
	{
		// 重新申请一块内存
		LPVOID pNewFileBuffer = malloc(sizeof(CHAR) * TrueSizeOfFileBuffer);

		if (pNewFileBuffer == NULL)
		{
			// 申请空间失败则返回 NULL
			return NULL;
		}

		// 即将申请的空间中的值置为0
		memset(pNewFileBuffer, 0x00000000, sizeof(CHAR) * TrueSizeOfFileBuffer);

		// 将数据复制过去
		memcpy(pNewFileBuffer, pFileBuffer, sizeof(CHAR) * TrueSizeOfFileBuffer);
		
		// 释放旧的 FileBuffer 缓冲区
		free(pFileBuffer);

		// 将 pFileBuffer 指针指向 NULL
		pFileBuffer = NULL;

		//返回 pNewFileBuffer 指针
		return pNewFileBuffer;
	}
}

// ************************************************************************

DWORD SizeOfCodeToInjectDll(IN LPVOID pFileBuffer, IN LPSTR DLL_PATH, IN UINT FunctionNameLength)
{
	/* 
		计算注入一个 DLL 至少需要多少空间
		需要移动的全部导入表 + 新增的导入表 + 全0结构的导入表 + \
		每个 DLL 导入表的子结构 ( IAT 表不能随意移动)
	*/ 
	
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

	// 定义 _IMAGE_THUNK_DATA32 结构体指针
	DWORD pPIMAGE_THUNK_DATA32 = (DWORD)(ChangeRvaToFoa(pPIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk, pFileBuffer) + (DWORD)pFileBuffer);

	// 定义 _IMAGE_IMPORT_BY_NAME 结构体指针
	PIMAGE_IMPORT_BY_NAME pPIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa(*(LPDWORD)pPIMAGE_THUNK_DATA32, pFileBuffer) + (DWORD)pFileBuffer);

	// 最终的代码字节数 = 所有 DLL 的导入表大小(包含空白结构的 20 个字节大小) + 新增的 20 个字节的导入表大小 + IAT + INT + (Dll名长度 + 1) + (函数名长度 + 2 + 1)
	DWORD dwSizeOfCodeToInjectDll = (pPIMAGE_DATA_DIRECTORY + 1) -> Size + sizeof(IMAGE_IMPORT_DESCRIPTOR) + \
		sizeof(IMAGE_THUNK_DATA32) * 4 + strlen(DLL_PATH) + sizeof(CHAR) + FunctionNameLength + sizeof(pPIMAGE_IMPORT_BY_NAME -> Hint) + sizeof(CHAR);


	return dwSizeOfCodeToInjectDll;
}

// ************************************************************************

LPVOID IsEnoughSpace(IN LPVOID pFileBuffer, IN UINT SizeOfCodeToInjectDll)
{
	/*
		判断是否有足够的空间写入数据
	*/

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

	// 声明 CodeBegin 指针
	LPVOID pCodeBegin = NULL;
	
	// 判断每个节的空白区是否大于需要写入的数据大小
	for (int i = 0; i < pPIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		// 这里特别需要注意 出现负数和正数的比较 要用有符号数
		if ((INT)(pPIMAGE_SECTION_HEADER -> SizeOfRawData - pPIMAGE_SECTION_HEADER -> Misc.VirtualSize) > (INT)SizeOfCodeToInjectDll)
		{
			pCodeBegin= (LPVOID)(pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> Misc.VirtualSize + (DWORD)pFileBuffer);
			break;
		}
		pPIMAGE_SECTION_HEADER++;
	}

	// 如果没有任何一个节有足够的空间写入数据则返回 NULL
	if (!pCodeBegin)
	{
		return NULL;
	}

	// 判断此节是否至少是可读可写的 如果不是则将其修改
	if (!(pPIMAGE_SECTION_HEADER -> Characteristics & IMAGE_SCN_MEM_READ))
	{
		// 不可读则改为可读
		pPIMAGE_SECTION_HEADER -> Characteristics &= IMAGE_SCN_MEM_READ;
	}
	
	if (!(pPIMAGE_SECTION_HEADER -> Characteristics & IMAGE_SCN_MEM_WRITE))
	{
		// 不可写则改为可写
		pPIMAGE_SECTION_HEADER -> Characteristics &= IMAGE_SCN_MEM_WRITE;
	}
	

	return pCodeBegin;
}

// ************************************************************************

DWORD SizeOfFileBuffer(IN LPVOID pFileBuffer)
{
	/* 根据 FileBuffer 计算出当前 FileBuffer 的大小 */

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

	// 将节表结构体指针偏移至最后一个节表
	pPIMAGE_SECTION_HEADER += (pPIMAGE_FILE_HEADER -> NumberOfSections - 1);

	// 当前的 FileBuffer 总大小 = 最后一个节的 PointerToRawData + SizeOfRawData
	DWORD dwSizeOfFileBuffer = pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> SizeOfRawData;
		
	return dwSizeOfFileBuffer;
}

// ************************************************************************

DWORD ImportDescriptorInjectDll(IN LPVOID pFileBuffer, IN LPVOID pCodeBegin, IN LPCSTR strDllFunctionNameAddr, IN LPSTR FileNameAddr, IN DWORD SizeOfFileBufferEx)
{
	/* 通过导入表注入 DLL */

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

	// 首先判断 pCodeBegin 是否为 NULL
	if (!pCodeBegin)
	{
		// 如果 pCodeBegin 的值为 NULL 则表明节的空闲区没有足够的空间
		return 0xFFFFFFFF;
	}

	// 判断 pCodeBegin 是否为 -1
	if ((DWORD)pCodeBegin == 0xFFFFFFFF)
	{	
		// 当前的要写入数据的位置 = SizeOfFileBufferEx + pFileBuffer
		LPVOID pNewCodeBegin = (LPVOID)(SizeOfFileBufferEx + (DWORD)pFileBuffer);

		// 修正数据目录中的导入表的指向
		(pPIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress = ChangeFoaToRva((DWORD)pNewCodeBegin - (DWORD)pFileBuffer, pFileBuffer);

		// 依次将导入表外层结构复制到新的位置
		do
		{
			// 判断是否导入表外层结构结束 结束则跳出
			if (!(pPIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pPIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
				pPIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pPIMAGE_IMPORT_DESCRIPTOR -> Name | \
				pPIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
				break;

			// 从 pPIMAGE_IMPORT_DESCRIPTOR 复制到 pNewCodeBegin
			memcpy(pNewCodeBegin, pPIMAGE_IMPORT_DESCRIPTOR, sizeof(CHAR) * sizeof(_IMAGE_IMPORT_DESCRIPTOR));

			// 找到下一个模块
			pPIMAGE_IMPORT_DESCRIPTOR++;

			// pNewCodeBegin 指针向后偏移一个结构体大小
			pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR));
		}
		while(TRUE);

		// 新的DLL的 INT表 的起始位置 FOA (跳过两个 _IMAGE_IMPORT_DESCRIPTOR 结构)
		LPVOID pINT_FOA = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2 - (DWORD)pFileBuffer);
		// 新的DLL的 IAT表 的起始位置 FOA
		LPVOID pIAT_FOA = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2 + sizeof(UINT) * 2 - (DWORD)pFileBuffer);
		// 新的DLL的 NAME 的起始位置 FOA
		LPVOID pNAME_FOA = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2 + sizeof(UINT) * 4 - (DWORD)pFileBuffer);

		// 新增一个 DLL 的导入表 修正成员的值
		PIMAGE_IMPORT_DESCRIPTOR pNEW_PIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)pNewCodeBegin;
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk = ChangeFoaToRva((DWORD)pINT_FOA, pFileBuffer);
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp = 0;
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain = 0;
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> Name = ChangeFoaToRva((DWORD)pNAME_FOA, pFileBuffer);
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> FirstThunk = ChangeFoaToRva((DWORD)pIAT_FOA, pFileBuffer);
		// 结尾有一个结构体的全0结构

		// 跳过新增的 DLL 导入表及一个全0结构
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2);

		// 保留此处的指针的值以备使用
		LPVOID pINT_F0A_ADD_FileBuffer = pNewCodeBegin;

		// 跳过新增的 INT 表
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(UINT) * 2);

		// 保留此处的指针的值以备使用
		LPVOID pIAT_F0A_ADD_FileBuffer = pNewCodeBegin;

		// 跳过新增的 IAT 表
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(UINT) * 2);

		// 得到 DLL 的名字的长度
		DWORD DllNameLength = strlen(FileNameAddr);

		// 添加 DLL 名称
		strcpy((LPSTR)pNewCodeBegin, (LPCSTR)FileNameAddr);

		// 跳过 DLL 名称的长度
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + DllNameLength);
		
		// 新增一个 _IMAGE_IMPORT_BY_NAME 结构
		PIMAGE_IMPORT_BY_NAME pPIMAGE_IMPORT_BY_NAME = (IMAGE_IMPORT_BY_NAME*)pNewCodeBegin;

		// _IMAGE_IMPORT_BY_NAME 中的 Hint 成员值可以为0
		pPIMAGE_IMPORT_BY_NAME -> Hint = 0;

		// 将 DLL 中的函数名称复制到指定位置
		strcpy((LPSTR)&pPIMAGE_IMPORT_BY_NAME -> Name, (LPCSTR)strDllFunctionNameAddr);

		// 修正 INT 表的值
		*(LPDWORD)pINT_F0A_ADD_FileBuffer = ChangeFoaToRva((DWORD)pNewCodeBegin - (DWORD)pFileBuffer, pFileBuffer);

		// 修正 IAT 表的值
		*(LPDWORD)pIAT_F0A_ADD_FileBuffer = ChangeFoaToRva((DWORD)pNewCodeBegin - (DWORD)pFileBuffer, pFileBuffer);

		return NULL;
	}
	else
	{
		// 如果 pCodeBegin 的值不为 NULL 和 -1 那么 pCodeBegin 的值就是将要进行写入数据的首地址
		LPVOID pNewCodeBegin = pCodeBegin;

		// 修正数据目录中的导入表的指向
		(pPIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress = ChangeFoaToRva((DWORD)pNewCodeBegin - (DWORD)pFileBuffer, pFileBuffer);

		// 依次将导入表外层结构复制到新的位置
		do
		{
			// 判断是否导入表外层结构结束 结束则跳出
			if (!(pPIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pPIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
				pPIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pPIMAGE_IMPORT_DESCRIPTOR -> Name | \
				pPIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
				break;

			// 从 pPIMAGE_IMPORT_DESCRIPTOR 复制到 pNewCodeBegin
			memcpy(pNewCodeBegin, pPIMAGE_IMPORT_DESCRIPTOR, sizeof(CHAR) * sizeof(_IMAGE_IMPORT_DESCRIPTOR));

			// 找到下一个模块
			pPIMAGE_IMPORT_DESCRIPTOR++;

			// pNewCodeBegin 指针向后偏移一个结构体大小
			pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR));
		}
		while(TRUE);

		// 新的DLL的 INT表 的起始位置 FOA (跳过两个 _IMAGE_IMPORT_DESCRIPTOR 结构)
		LPVOID pINT_FOA = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2 - (DWORD)pFileBuffer);
		// 新的DLL的 IAT表 的起始位置 FOA
		LPVOID pIAT_FOA = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2 + sizeof(UINT) * 2 - (DWORD)pFileBuffer);
		// 新的DLL的 NAME 的起始位置 FOA
		LPVOID pNAME_FOA = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2 + sizeof(UINT) * 4 - (DWORD)pFileBuffer);

		// 新增一个 DLL 的导入表 修正成员的值
		PIMAGE_IMPORT_DESCRIPTOR pNEW_PIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)pNewCodeBegin;
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk = ChangeFoaToRva((DWORD)pINT_FOA, pFileBuffer);
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp = 0;
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain = 0;
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> Name = ChangeFoaToRva((DWORD)pNAME_FOA, pFileBuffer);
		pNEW_PIMAGE_IMPORT_DESCRIPTOR -> FirstThunk = ChangeFoaToRva((DWORD)pIAT_FOA, pFileBuffer);
		// 结尾有一个结构体的全0结构

		// 跳过新增的 DLL 导入表及一个全0结构
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(_IMAGE_IMPORT_DESCRIPTOR) * 2);

		// 保留此处的指针的值以备使用
		LPVOID pINT_F0A_ADD_FileBuffer = pNewCodeBegin;

		// 跳过新增的 INT 表
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(UINT) * 2);

		// 保留此处的指针的值以备使用
		LPVOID pIAT_F0A_ADD_FileBuffer = pNewCodeBegin;

		// 跳过新增的 IAT 表
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + sizeof(UINT) * 2);

		// 得到 DLL 的名字的长度
		DWORD DllNameLength = strlen(FileNameAddr);

		// 添加 DLL 名称
		strcpy((LPSTR)pNewCodeBegin, (LPCSTR)FileNameAddr);

		// 跳过 DLL 名称的长度
		pNewCodeBegin = (LPVOID)((DWORD)pNewCodeBegin + DllNameLength);
		
		// 新增一个 _IMAGE_IMPORT_BY_NAME 结构
		PIMAGE_IMPORT_BY_NAME pPIMAGE_IMPORT_BY_NAME = (IMAGE_IMPORT_BY_NAME*)pNewCodeBegin;

		// _IMAGE_IMPORT_BY_NAME 中的 Hint 成员值可以为0
		pPIMAGE_IMPORT_BY_NAME -> Hint = 0;

		// 将 DLL 中的函数名称复制到指定位置
		strcpy((LPSTR)&pPIMAGE_IMPORT_BY_NAME -> Name, (LPCSTR)strDllFunctionNameAddr);

		// 修正 INT 表的值
		*(LPDWORD)pINT_F0A_ADD_FileBuffer = ChangeFoaToRva((DWORD)pNewCodeBegin - (DWORD)pFileBuffer, pFileBuffer);

		// 修正 IAT 表的值
		*(LPDWORD)pIAT_F0A_ADD_FileBuffer = ChangeFoaToRva((DWORD)pNewCodeBegin - (DWORD)pFileBuffer, pFileBuffer);

		return NULL;
	}

}

// ************************************************************************

LPVOID ExpandSection(IN UINT IsSuccess, IN LPVOID pFileBuffer, IN UINT SizeOfCodeAlignment)
{
	/*
		根据对齐参数的大小扩大节
	*/
	
	// 判断输入进来的 IsSuccess
	if (!IsSuccess)
	{
		// 如果输入进来的 IsSuccess 的值是 NULL 那么不进行扩大节
		return pFileBuffer;
	}

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

	// 获取当前的 FileBuffer 大小
	// 定位到最后一个节表
	pPIMAGE_SECTION_HEADER += (pPIMAGE_FILE_HEADER -> NumberOfSections - 1);

	// FileBuffer 大小
	DWORD SizeOfFileBuffer = pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> SizeOfRawData;
	
	// 扩大后的 NewFileBuffer 大小 = FileBuffer + SizeOfCodeAlignment
	DWORD SizeOfNewFileBuffer = pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> SizeOfRawData + SizeOfCodeAlignment;

	// 申请新的空间
	LPVOID pNewFileBuffer = malloc(sizeof(CHAR) * SizeOfNewFileBuffer);

	if (pNewFileBuffer == NULL)
	{
		// 失败
		printf(" NewFileBuffer 空间创建失败");
		return NULL;
	}

	// 初始化为零
	memset(pNewFileBuffer, 0x00000000, sizeof(CHAR) * SizeOfNewFileBuffer);

	// 将当前数据复制到新的位置
	memcpy(pNewFileBuffer, pFileBuffer, sizeof(CHAR) * SizeOfFileBuffer);

	// 释放当前的空间 初始化 pFileBuffer 指针
	free(pFileBuffer);
	pFileBuffer = NULL;

	// 将 PE 指针重新定向

	// 定义DOS头结构体指针
	pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pNewFileBuffer;

	// 定义NT头结构体指针
	pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));
	
	// 定义可选PE头结构体指针
	pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 校准数据
	// 跳转到最后一个节
	pPIMAGE_SECTION_HEADER += (pPIMAGE_FILE_HEADER -> NumberOfSections - 1);
	
	// Name 的值不需要改

	// VirtualAddress 的值不变

	// SizeOfRawData 的值改为 SizeOfRawData + AlignNumber
	pPIMAGE_SECTION_HEADER -> SizeOfRawData = pPIMAGE_SECTION_HEADER -> SizeOfRawData + SizeOfCodeAlignment;

	// Misc 的值更改为和当前的 SizeOfRawData 一致
	pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = pPIMAGE_SECTION_HEADER -> SizeOfRawData;

	// PointerToRawData 的值不变

	// 扩大节的属性改为 可读 / 可写 / 可执行 / 包含可执行代码
    pPIMAGE_SECTION_HEADER -> Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE;

	// 更改 SizeOfImage 中的值
	pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage += SizeOfCodeAlignment;


	return pNewFileBuffer;
}

// ************************************************************************

LPSTR DllFunctionName(IN LPSTR DLL_PATH)
{
	/*
		根据给定的 DLL 路径获取其中的第一个函数名
	*/

	// 根据文件大小申请一块内存
	LPVOID pFileBuffer_DLL = CreateFileBuffer(DLL_PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD SizeOf_ReadPeFileToFileBuffer = ReadPeFileToFileBuffer(DLL_PATH, pFileBuffer_DLL);
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer_DLL;

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

	// 如果不是 DLL 文件则返回 NULL 
	if (!(pPIMAGE_FILE_HEADER -> Characteristics & 0x2000))
	{
		printf("这不是一个有效的 DLL 文件");
		return NULL;
	}

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pPIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	
	// 定义导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pFileBuffer_DLL) + (DWORD)pFileBuffer_DLL);
	
	// 判断此 DLL 是否有导出的函数 没有则返回 NULL
	if (!pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames)
	{
		// 目前仅支持以名字导出的函数的 DLL 进行注入
		printf("此 DLL 没有以名字导出的函数");
		return NULL;
	}
		
	// 如果有以名字导出的函数则找到函数名地址的地址
	LPDWORD dwFunctionNameAddr = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames, pFileBuffer_DLL) + (DWORD)pFileBuffer_DLL);
	
	// 得到函数名的地址并存储至常量区中
	LPSTR strFunctionNameLength = (LPSTR)(ChangeRvaToFoa(*dwFunctionNameAddr, pFileBuffer_DLL) + (DWORD)pFileBuffer_DLL);

	return strFunctionNameLength;
}

// ************************************************************************

LPSTR GetFileName(IN LPSTR FilePath)
{
	/* 
		根据传入的文件路径获取文件名称并返回
	*/

	if (!FilePath)
	{
		return NULL;
	}

	DWORD strLength = strlen(FilePath);
	LPSTR pFileName = NULL;

	for (UINT i = 0; i < strLength; i++)
	{
		if (*FilePath++ == '\\')
		{
			pFileName = FilePath;
		}
	}

	return pFileName;
}


// stdafx.cpp : source file that includes just the standard includes
//	TLS_TABLE.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

//////////////////////////////////////////////////////////////////////
// 此文件提供处理 PE 文件的函数
//////////////////////////////////////////////////////////////////////

DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer)
{
	/*
		此函数为32位PE文件RVA转FOA
		此函数接收一个 RVA 及一个 FileBuffer 指针 返回一个 FOA
		如果执行成功则返回该值在文件中的偏移 FOA 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 声明一个 Foa 用于存储最终的结果
	DWORD FOA = 0;

	// 计算 Rva 与 pImageBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0;

	// 判断输入进来的 Rva 将落在 ImageBuffer 中的哪一个节里
	if (RVA < pIMAGE_SECTION_HEADER -> VirtualAddress)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		FOA = RVA;
		return FOA;
	}

	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (RVA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress)
		{
			// 计算 Rva 与最后一个节的 VirtualAddress 之间的偏移
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress;

			// Foa 的值为 最后一个节的 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData + TempNumber;
		}

		// 如果落在除最后一个节外的其它节里
		if (RVA >= (pIMAGE_SECTION_HEADER + i) -> VirtualAddress && RVA < (pIMAGE_SECTION_HEADER + i + 1) -> VirtualAddress)
		{
			// 计算 Rva 与 VirtualAddress 之间的偏移
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + i) -> VirtualAddress;

			// Foa 的值为 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pIMAGE_SECTION_HEADER + i) -> PointerToRawData + TempNumber;
			
		}
	}

	return FOA;
}

//////////////////////////////////////////////////////////////////////
DWORD ChangeRvaToFoa64(IN DWORD RVA, IN LPVOID fpFileBuffer)
{
	/*
		此函数为64位PE文件RVA转FOA
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER64 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 声明一个 Foa 用于存储最终的结果
	DWORD FOA = 0;

	// 计算 Rva 与 pImageBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0;

	// 判断输入进来的 Rva 将落在 ImageBuffer 中的哪一个节里
	if (RVA < pIMAGE_SECTION_HEADER -> VirtualAddress)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		FOA = RVA;
		return FOA;
	}

	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (RVA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress)
		{
			// 计算 Rva 与最后一个节的 VirtualAddress 之间的偏移
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress;

			// Foa 的值为 最后一个节的 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData + TempNumber;
		}

		// 如果落在除最后一个节外的其它节里
		if (RVA >= (pIMAGE_SECTION_HEADER + i) -> VirtualAddress && RVA < (pIMAGE_SECTION_HEADER + i + 1) -> VirtualAddress)
		{
			// 计算 Rva 与 VirtualAddress 之间的偏移
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + i) -> VirtualAddress;

			// Foa 的值为 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pIMAGE_SECTION_HEADER + i) -> PointerToRawData + TempNumber;
			
		}
	}

	return FOA;
}

//////////////////////////////////////////////////////////////////////

DWORD ChangeFoaToRva32(IN UINT FOA, IN LPVOID fpFileBuffer)
{
	/*
		此函数接收一个 FOA 及一个 FileBuffer 指针 返回一个 RVA
		如果执行成功则返回该值在文件中的偏移 RVA 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 声明一个 Rva 用于存储最终的结果
	DWORD RVA = 0;

	// 计算 Foa 与 pFileBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0; 

	// 判断输入进来的 Foa 将落在 FileBuffer 中的哪一个节里
	if (FOA < pIMAGE_SECTION_HEADER -> PointerToRawData)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		RVA = FOA;
		return RVA;
	}

	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (FOA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData)
		{
			// 计算 Foa 与最后一个节的 PointerToRawData 之间的偏移
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData;

			// RVA 的值为 最后一个节的 VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress + TempNumber;
		}

		// 如果落在除最后一个节外的其它节里
		if (FOA >= (pIMAGE_SECTION_HEADER + i) -> PointerToRawData && FOA < (pIMAGE_SECTION_HEADER + i + 1) -> PointerToRawData)
		{
			// 计算 FOA 与 PointerToRawData 之间的偏移
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + i) -> PointerToRawData;

			// RVA 的值为 VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + i) -> VirtualAddress + TempNumber;
			
		}
	}

	return RVA;
}

//////////////////////////////////////////////////////////////////////
DWORD ChangeFoaToRva64(IN UINT FOA, IN LPVOID fpFileBuffer)
{
	/*
		此函数为64位PE文件FOA转RVA
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER64 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 声明一个 Rva 用于存储最终的结果
	DWORD RVA = 0;

	// 计算 Foa 与 pFileBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0; 

	// 判断输入进来的 Foa 将落在 FileBuffer 中的哪一个节里
	if (FOA < pIMAGE_SECTION_HEADER -> PointerToRawData)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		RVA = FOA;
		return RVA;
	}

	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (FOA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData)
		{
			// 计算 Foa 与最后一个节的 PointerToRawData 之间的偏移
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData;

			// RVA 的值为 最后一个节的 VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress + TempNumber;
		}

		// 如果落在除最后一个节外的其它节里
		if (FOA >= (pIMAGE_SECTION_HEADER + i) -> PointerToRawData && FOA < (pIMAGE_SECTION_HEADER + i + 1) -> PointerToRawData)
		{
			// 计算 FOA 与 PointerToRawData 之间的偏移
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + i) -> PointerToRawData;

			// RVA 的值为 VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + i) -> VirtualAddress + TempNumber;
			
		}
	}

	return RVA;
}

//////////////////////////////////////////////////////////////////////

DWORD IsPeFile(IN LPTSTR lpszFilePath, OUT LPVOID* lpFileBuffer)
{
	/*
		检测文件是否是PE文件
		如果是PE文件则根据有效的PE文件大小进行读取
	*/

	// 以读二进制的方式打开文件流
	FILE* lpszFileStream = fopen(lpszFilePath, "rb");
	if (!lpszFileStream)
	{
		// 返回空指针则代表文件打开出现错误
		return ERROR_OPEN_FILE;
	}

	// 用于存储需要判断的数据
	DWORD dwSignature = 0;

	// 首先读取2个字节
	DWORD dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream);
	if (dwFreadCount != sizeof(WORD))
	{
		// 文件读取失败
		return ERROR_READ_FILE;
	}
	
	// 检查是否是有效的DOS_MZ标志
	if (LOWORD(dwSignature) != IMAGE_DOS_SIGNATURE)
	{
		// 错误的DOS_MZ头
		return ERROR_DOS_MZ;
	}

	// 如果DOS_MZ头的值正确,则将文件指针向后偏移0x3A的长度
	fseek(lpszFileStream, 0x3A, SEEK_CUR);

	// 读取4个字节的长度,得到e_lfanew的值
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream);
	if (dwFreadCount != sizeof(DWORD))
	{
		return ERROR_READ_FILE;
	}
	
	// 文件指针从文件开头向后偏移e_lfanew的大小
	fseek(lpszFileStream, dwSignature, SEEK_SET);

	// 再读取4个字节的长度,得到IMAGE_NT_SIGNATURE
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream);
	if (dwFreadCount != sizeof(DWORD))
	{
		return ERROR_READ_FILE;
	}

	// 检测PE标志是否正确
	if (dwSignature != IMAGE_NT_SIGNATURE)
	{
		// 错误的PE标志
		return ERROR_PE_SIGNATURE;
	}

	//	文件指针再向后偏移20个字节
	fseek(lpszFileStream, IMAGE_SIZEOF_FILE_HEADER, SEEK_CUR);

	// 读取2个字节
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream);
	if (dwFreadCount != sizeof(WORD))
	{
		return ERROR_READ_FILE;
	}

	// 默认是32位的程序

	// 取出SizeOfHeaders的值
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream);
	if (dwFreadCount != sizeof(DWORD))
	{
		return ERROR_READ_FILE;
	}
	
	// 先根据此大小申请空间
	LPVOID lpszFileBuffer = malloc(sizeof(CHAR) * dwSignature);
	if (!lpszFileBuffer)
	{
		// 内存申请失败
		return ERROR_MEMORY_REQUEST_FAILURE;
	}
	memset(lpszFileBuffer, 0x00000000, sizeof(CHAR) * dwSignature);
	fseek(lpszFileStream, 0, SEEK_SET);
	fread(lpszFileBuffer, sizeof(CHAR), sizeof(CHAR) * dwSignature, lpszFileStream);

	// 根据32位PE文件定义指针

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpszFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 转到最后一个节
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	
	// 得到最后一个节的PointerToRawData + SizeOfRawData的大小
	dwSignature = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;

	// 释放当前空间
	free(lpszFileBuffer);

	// 如果PE文件的实际有效大小大于1G则返回
	if (dwSignature >= FILE_MAX_SIZE)
	{
		return ERROR_PE_FILE_TOO_BIG;
	}
	
	// 重新申请有效的PE文件大小+0x10个字节(以防止重定位表解析失败)
	lpszFileBuffer = malloc(sizeof(CHAR) * dwSignature + 0x10);
	if (!lpszFileBuffer)
	{
		// 内存申请失败
		return ERROR_MEMORY_REQUEST_FAILURE;
	}
	memset(lpszFileBuffer, 0x00000000, sizeof(CHAR) * dwSignature + 0x10);
	fseek(lpszFileStream, 0, SEEK_SET);
	fread(lpszFileBuffer, sizeof(CHAR), sizeof(CHAR) * dwSignature, lpszFileStream);

	// 将文件指针返回至指定的位置
	*(LPDWORD*)lpFileBuffer = (LPDWORD)lpszFileBuffer;

	// 关闭文件流
	dwFreadCount = fclose(lpszFileStream);
	if (dwFreadCount)
	{
		// 文件关闭失败
		free(lpszFileBuffer);
		lpszFileBuffer = NULL;
		return ERROR_CLOSE_FILE;
	}

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////



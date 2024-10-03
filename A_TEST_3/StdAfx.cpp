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

LPVOID CreateImageBuffer(IN LPVOID pFileBuffer)
{
	/*
		根据给定的 pFileBuffer 指针读取内存中的内容
		需要先判断是否是PE文件
		如果是PE文件则根据 可选PE头中 SizeOfImage 的大小来申请内存空间
		如果执行成功则返回所申请的空间的 pImageBuffer 失败则返回0
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
		printf("这不是一个有效的PE文件: 错误的MZ头标志");
		return NULL;
	}

	// 判断PE标识是否有效
	if (*(PDWORD)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew) != IMAGE_NT_SIGNATURE)
	{
		printf("这不是一个有效的PE文件: 错误的PE标志");
		return NULL;
	}
	LPVOID pImageBuffer = malloc(sizeof(CHAR) * pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage); 
	if (pImageBuffer == NULL)
	{
		printf(" ImageBuffer 空间申请失败\n");
		return NULL;
	}

	return pImageBuffer;
}

// ************************************************************************

LPVOID CreateNewBuffer(IN LPVOID pImageBuffer)
{
	/*
		根据给定的 ImageBuffer 计算得出所压缩后的 NewBuffer 的空间大小
		并根据此大小申请对应的空间
		如果执行成功则返回所申请的空间的指针 pNewBuffer 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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
	
	// SizeOfNewBuffer 最终的值为将要申请的内存空间大小
	DWORD SizeOfNewBuffer = pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders;
	for (int i = 0; i < pPIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		SizeOfNewBuffer += pPIMAGE_SECTION_HEADER -> SizeOfRawData;
		pPIMAGE_SECTION_HEADER++;
	}

	LPVOID pNewBuffer = malloc(sizeof(CHAR) * SizeOfNewBuffer); 
	if (pNewBuffer == NULL)
	{
		printf(" NewBuffer 空间申请失败");
		return NULL;
	}

	return pNewBuffer;
}

// ************************************************************************

LPVOID CreateNewImageBuffer(IN LPVOID pImageBuffer, IN UINT SizeOfCode)
{
	/*
		创建新增节所需要的空间
		根据给定的 pImageBuffer 指针 计算得出是否有足够的空间添加一个节表 
		如果有足够的空间添加一个节表 则根据所提供的 SizeOfCode 的大小
		计算得出其内存对齐后 整个 PE 文件在内存中的总大小 并以此申请相同大小的空间
		如果执行成功则返回所申请的空间的void*类型的指针 pNewImageBuffer 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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

	// 计算新增节表的空间是否足够 最终结果存储在 SizeOfAddNewSection 中
	DWORD SizeOfAddNewSection = 0;
	
	// 累加DOS块中的空间  64为DOS头的大小
	SizeOfAddNewSection += ((DWORD)pPIMAGE_NT_HEADERS32 - (DWORD)pPIMAGE_DOS_HEADER - (DWORD)64);
 
	// 累加最后一个节表后的可用空间 
	DWORD TempNumber = (pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders -  ((DWORD)pPIMAGE_SECTION_HEADER - (DWORD)pPIMAGE_DOS_HEADER + IMAGE_SIZEOF_SECTION_HEADER * pPIMAGE_FILE_HEADER -> NumberOfSections));

	for (UINT i = 0; i < TempNumber; i++)
	{
		if ((*((PUCHAR)(((DWORD)pPIMAGE_SECTION_HEADER) + IMAGE_SIZEOF_SECTION_HEADER * pPIMAGE_FILE_HEADER -> NumberOfSections) + i)))
		{
			// 如果最后一个节表后有值则跳出循环 不再累加
			break;
		}
		SizeOfAddNewSection += 1;
	}

	// 如果总空间不足80个字节则返回NULL
	if (SizeOfAddNewSection < IMAGE_SIZEOF_SECTION_HEADER * 2)
	{
		// 空间不足 无法新增节 返回NULL
		printf("此PE文件 空间不足以新增一个节");
		return NULL;
	}

	// 如果空间足够新增一个节表 则开始计算最终的 SizeOfImage 的大小
	// SizeOfCodeSectionAlignment 根据内存对齐的大小 计算此段代码需要分配多少空间  
	DWORD SizeOfCodeSectionAlignment = 0x00000000;
	if (!(SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
	{
		// 如果内存对齐的大小 大于要写入的代码的大小 则要添加的代码大小根据内存对齐的大小来算
		SizeOfCodeSectionAlignment = pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
	}
	if (SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
	{
		// 如果内存对齐的大小 小于要写入的代码的大小 则需要进行计算  
		if (!(SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
		{
			// 如果刚好是内存对齐的整数倍 则要写入的代码的大小就等于此大小
			SizeOfCodeSectionAlignment = SizeOfCode;
		}
		if (SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
		{
			// 如果输入的代码长度 大于内存对齐的值 并且不是内存对齐的整数倍 
			// 则要写入的代码的大小 等于输入的代码大小除以内存对齐的值的结果 + 1 再乘以内存对齐的参数
			SizeOfCodeSectionAlignment =  ((SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment) + 1) * pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
		}
	}

	// 此时的 SizeOfCodeSectionAlignment 为真正要添加的节的数据的大小
	// 最终需要申请的内存空间的大小则等于 SizeOfCodeSectionAlignment + SizeOfImage 的大小
	SizeOfCodeSectionAlignment += pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;

	// 根据最终确定的大小申请 NewImageBuffer 的空间
	LPVOID pNewImageBuffer = malloc(sizeof(CHAR) * SizeOfCodeSectionAlignment);
	if (pNewImageBuffer == NULL)
	{
		printf("pNewImageBuffer 空间申请失败\n");
		return NULL;
	}

	// 将申请的空间内的数据初始化为 0
	memset(pNewImageBuffer, 0x00000000, sizeof(CHAR) * SizeOfCodeSectionAlignment);

	return pNewImageBuffer;
}

// ************************************************************************


LPVOID CreateNewImageBufferEx(IN LPVOID pImageBuffer, IN UINT Size)
{
	/*
		根据所提供的 pImageBuffer 及 Size 来申请一块空间
		如果执行成功则返回所申请的空间的指针 pNewImageBuffer 失败则返回0
	*/ 

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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
	
	LPVOID pNewImageBuffer = malloc(sizeof(CHAR) * (pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage + Size));
	if (pNewImageBuffer == NULL)
	{
		// 申请空间失败返回NULL
		printf("空间申请失败");
		return NULL;
	}

	// 将申请的空间内的数据初始化为 0
	memset(pNewImageBuffer, 0x00000000, sizeof(CHAR) * (pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage + Size));

	return pNewImageBuffer;
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

DWORD CopyFileBufferToImageBuffer(IN LPVOID pFileBuffer, OUT LPVOID pImageBuffer)
{
	/*
		需要先判断是否是PE文件
		根据所给的指针 从 FileBuffer 将数据复制到 ImageBuffer 中
		如果拷贝成功则返回所拷贝的字节数大小 失败则返回0
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
		printf("这不是一个有效的PE文件: 错误的MZ头标志");
		return NULL;
	}
	// 判断PE标识是否有效
	if (*(PDWORD)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew) != IMAGE_NT_SIGNATURE)
	{
		printf("这不是一个有效的PE文件: 错误的PE标志");
		return NULL;
	}
	// 定义将返回的值 拷贝的字节数大小
	DWORD SizeOfCopyByte = 0x00000000;

	// 将PE文件头部(所有头加节表按照文件对齐的大小)从 FileBuffer 拷贝到 ImageBuffer
	memcpy(pImageBuffer, pFileBuffer, pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);

	// 计算所有头加节表按照文件对齐后拷贝的大小
	SizeOfCopyByte += pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders;

	// 循环将PE文件 节数据拷贝到对应位置
	for (int i = 0; i < pPIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		// 从 pFileBuffer + PointerToRawData 的位置 拷贝到 pImageBuffer + VirtualAddress 的位置
		memcpy(LPVOID((DWORD)pImageBuffer + pPIMAGE_SECTION_HEADER -> VirtualAddress), \
			LPVOID((DWORD)pFileBuffer + pPIMAGE_SECTION_HEADER -> PointerToRawData), \
			pPIMAGE_SECTION_HEADER -> SizeOfRawData);
		// 计算每个节文件对齐后拷贝的大小
		SizeOfCopyByte += pPIMAGE_SECTION_HEADER -> SizeOfRawData;
		pPIMAGE_SECTION_HEADER++;
	}

	return SizeOfCopyByte;
}

// ************************************************************************

DWORD CopyImageBufferToNewBuffer(IN LPVOID pImageBuffer,OUT LPVOID pNewBuffer)
{
	/*
		需要先判断是否是PE文件
		如果是PE文件 则根据所给的指针 从 ImageBuffer 将数据复制到 NewBuffer 中
		如果拷贝成功则返回所拷贝的字节数大小 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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
	// 定义将返回的值 拷贝的字节数大小
	DWORD SizeOfCopyByte = 0x00000000;

	// 将PE文件头部(所有头加节表按照文件对齐的大小)从 ImageBuffer 拷贝到 NewBuffer
	memcpy(pNewBuffer, pImageBuffer, pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);

	// 计算所有头加节表按照文件对齐后拷贝的大小
	SizeOfCopyByte += pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders;

	// 循环将PE文件 节数据拷贝到对应位置
	for (int i = 0; i < pPIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		// 从 pImageBuffer + VirtualAddress 的位置 的位置 拷贝到 pNewBuffer + PointerToRawData 的位置
		memcpy(LPVOID((DWORD)pNewBuffer + pPIMAGE_SECTION_HEADER -> PointerToRawData), \
			LPVOID((DWORD)pImageBuffer + pPIMAGE_SECTION_HEADER -> VirtualAddress), \
			pPIMAGE_SECTION_HEADER -> SizeOfRawData);
		// 计算每个节文件对齐后拷贝的大小
		SizeOfCopyByte += pPIMAGE_SECTION_HEADER -> SizeOfRawData;
		pPIMAGE_SECTION_HEADER++;
	}

	return SizeOfCopyByte;
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
BOOL AddCodeToImageBuffer(IN PUCHAR pCodeArray,IN UINT HowNumberOfSections, IN UINT SizeOfArray, OUT LPVOID pImageBuffer)
{
	/*
		此函数用于静态注入代码
		根据给定的 指向数组的指针 及 pImageBuffer指针 进行代码的插入
		需要同时指定将代码插入到哪一个节中 并指明数组的大小
		如果代码插入成功返回TRUE 失败则返回0

	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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

	// 判断是否有此节
	if ((HowNumberOfSections > pPIMAGE_FILE_HEADER -> NumberOfSections) || (HowNumberOfSections <= 0))
	{
		printf("没有此节\n");
		return NULL;
	}

	// 处理输入进来的数值 让其减一
	HowNumberOfSections -= 1;

	// 节表结构体指针跳转到需要添加代码的节
	pPIMAGE_SECTION_HEADER += HowNumberOfSections; 

	// 计算此节是否有足够的空间写入代码
	if ((INT)(pPIMAGE_SECTION_HEADER -> SizeOfRawData - pPIMAGE_SECTION_HEADER -> Misc.VirtualSize) < (INT)SizeOfArray)
	{
		// 没有足够的空间则退出
		printf("此节没有足够的空间添加代码\n");
		return NULL;
	}
	
	// 判断此节的属性 确保其为可执行的 如果不是则将其更改
	DWORD TempCharacter = pPIMAGE_SECTION_HEADER -> Characteristics;
	if (!((TempCharacter & IMAGE_SCN_MEM_EXECUTE) == IMAGE_SCN_MEM_EXECUTE))
	{
		pPIMAGE_SECTION_HEADER -> Characteristics |= IMAGE_SCN_MEM_EXECUTE;
	}
	
	// 找到要写入代码的位置
	PUCHAR CODE_BEGIN = (PUCHAR)(pPIMAGE_SECTION_HEADER -> VirtualAddress + pPIMAGE_SECTION_HEADER -> Misc.VirtualSize + (DWORD)pImageBuffer);

	// 先将数组的值写入指定位置
	for (UINT i = 0; i < SizeOfArray; i++)
	{
		*(CODE_BEGIN + i) = *(pCodeArray + i);
	}
	
	// 校对 call 和 jmp 后面的地址  (本机(Win10)的 MessageBoxA 函数地址: 0x760D09A0 / 0x760E09A0)
	// 真正要跳转的地址 = E8/E9 的下一行地址 + X
	// 即: X = 真正要跳转的地址 - E8/E9 的下一行地址
	// 此时的 E8/E9 的当前地址 应该取偏移地址 + ImageBase
	
	// 获取 CODE_BEGIN 在内存中的偏移地址 + ImageBase 
	DWORD VIRTUAL_CODE_BEGIN = ((DWORD)CODE_BEGIN - (DWORD)pImageBuffer) + pPIMAGE_OPTIONAL_HEADER32 -> ImageBase;

	// 先校准 E8 后面的地址 
	DWORD E8_Calibration = ToMessageBoxA - (VIRTUAL_CODE_BEGIN + 0x8 + 0x5);
	// 校准 E9 后面的地址 
	DWORD E9_Calibration = pPIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint + pPIMAGE_OPTIONAL_HEADER32 -> ImageBase - (VIRTUAL_CODE_BEGIN + 0x8 + 0x5 + 0x5);

	// 将 校准后的 E8/E9后面的值 写入 ImageBuffer 对应位置
	*(LPDWORD)(CODE_BEGIN + 0x9) = E8_Calibration;
	*(LPDWORD)(CODE_BEGIN + 0xE) = E9_Calibration;

	//更改入口点的值 为当前的 代码开始的偏移值
	pPIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint =  pPIMAGE_SECTION_HEADER -> VirtualAddress + pPIMAGE_SECTION_HEADER -> Misc.VirtualSize;

	return TRUE;
}

// ************************************************************************

BOOL CopyImageBufferToNewImageBuffer(IN LPVOID pImageBuffer, OUT LPVOID pNewImageBuffer, IN UINT SizeOfCode, IN UINT IsExistMisc)
{
	/*
		此函数用于新增节
		将 ImageBuffer 的数据复制到 NewImageBuffer 中
		校对所有需要修正的PE头里的值 并向上移动PE头和节表
		如果拷贝成功则返回TURE 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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

	// 将 ImageBuffer 的内容复制到 NewImageBuffer 中
	memcpy(pNewImageBuffer, pImageBuffer, sizeof(CHAR) * pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);

	// 重新给结构体指针赋值为 NewImageBuffer 中的地址

	// DOS头结构体指针
	pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pNewImageBuffer;

	// NT头结构体指针
	pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 标准PE头结构体指针
	pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));

	// 可选PE头结构体指针
	pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 节表结构体指针
	pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);


	// SizeOfCodeSectionAlignment 根据内存对齐的大小 计算此段代码需要分配多少空间  
	DWORD SizeOfCodeSectionAlignment = 0x00000000;
	if (!(SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
	{
		// 如果内存对齐的大小 大于要写入的代码的大小 则要添加的代码大小根据内存对齐的大小来算
		SizeOfCodeSectionAlignment = pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
	}
	if (SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
	{
		// 如果内存对齐的大小 小于要写入的代码的大小 则需要进行计算  
		if (!(SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
		{
			// 如果刚好是内存对齐的整数倍 则要写入的代码的大小就等于此大小
			SizeOfCodeSectionAlignment = SizeOfCode;
		}
		if (SizeOfCode % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
		{
			// 如果输入的代码长度 大于内存对齐的值 并且不是内存对齐的整数倍 
			// 则要写入的代码的大小 等于输入的代码大小除以内存对齐的值的结果 + 1 再乘以内存对齐的参数
			SizeOfCodeSectionAlignment =  ((SizeOfCode / pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment) + 1) * pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
			// 此时的 SizeOfCodeSectionAlignment 为真正要添加的节的数据的大小
		}
	}

	// 计算微软DOS块的大小 最终结果存储在 SizeOfAddNewSection 中 64为DOS头的大小
	DWORD MS_DOS_STUB = 0;
	MS_DOS_STUB += ((DWORD)pPIMAGE_NT_HEADERS32 - (DWORD)pPIMAGE_DOS_HEADER - (DWORD)64);

	// 将DOS块的空间完全覆盖 
	// 要复制到的位置
	PUCHAR pTempNewPeHeader = (PUCHAR)((DWORD)pPIMAGE_NT_HEADERS32 - MS_DOS_STUB);
	// 从NT头开始复制
	PUCHAR pTempOldPeHeader = (PUCHAR)(pPIMAGE_NT_HEADERS32);

	// 计算要复制的大小
	DWORD TempSize = sizeof(pPIMAGE_NT_HEADERS32 -> Signature) + IMAGE_SIZEOF_FILE_HEADER + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader + pPIMAGE_FILE_HEADER -> NumberOfSections * IMAGE_SIZEOF_SECTION_HEADER;
	// 将整个 PE 头加节表向上提升 
	memcpy((LPVOID)pTempNewPeHeader, (LPVOID)pTempOldPeHeader, TempSize);

	// e_lfanew 指向它的后一个字节
	pPIMAGE_DOS_HEADER -> e_lfanew = (DWORD)64;

	// 为除DOS头结构体指针外的其它结构体指针重新赋值

	// NT头结构体指针
	pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 标准PE头结构体指针
	pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));

	// 可选PE头结构体指针
	pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 节表结构体指针
	pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);


	// 新增节表(将新增的一个节表和它后面的一个节表的空间置为0)
	pPIMAGE_SECTION_HEADER += pPIMAGE_FILE_HEADER -> NumberOfSections;

	for (int i = 0; i < IMAGE_SIZEOF_SECTION_HEADER * 2; i++)
	{
		// 总共 80 个字节全部置为 0
		*((PUCHAR)pPIMAGE_SECTION_HEADER + i) = 0;
	}
	
	// 校对数据 目前 pPIMAGE_SECTION_HEADER 节表指针刚好指向新增的节

	// 新增节的名字
	pPIMAGE_SECTION_HEADER -> Name[0] = '.';
	pPIMAGE_SECTION_HEADER -> Name[1] = 'M';
	pPIMAGE_SECTION_HEADER -> Name[2] = 'y';
	pPIMAGE_SECTION_HEADER -> Name[3] = 'C';
	pPIMAGE_SECTION_HEADER -> Name[4] = 'o';
	pPIMAGE_SECTION_HEADER -> Name[5] = 'd';
	pPIMAGE_SECTION_HEADER -> Name[6] = 'e';
	pPIMAGE_SECTION_HEADER -> Name[7] = '\0';

	// 新增节对齐前的尺寸 根据传入的 IsExistMisc 的值来写入
	// 0代表 Misc 的值为0 输入其它值则代表此值等于SizeOfRawData
	if (!IsExistMisc)
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = 0;
	}
	else
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = SizeOfCodeSectionAlignment;
	}

	// 新增节在内存中的时候的偏移
	pPIMAGE_SECTION_HEADER -> VirtualAddress = pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;

	// 新增节的 SizeOfRawData
	pPIMAGE_SECTION_HEADER -> SizeOfRawData = SizeOfCodeSectionAlignment;

	// 新增节的 PointerToRawData 等于上一个节的 PointerToRawData + 上一个节的 Misc.VirtualSize
	pPIMAGE_SECTION_HEADER--;
	DWORD TempPointerToRawData = pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> SizeOfRawData;
	pPIMAGE_SECTION_HEADER++;
	pPIMAGE_SECTION_HEADER -> PointerToRawData = TempPointerToRawData;

	// 新增节的属性改为 可读 / 可执行 / 包含可执行代码
    pPIMAGE_SECTION_HEADER -> Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE;

	// 将标准PE头的 NumberOfSections 成员的节数加1
	pPIMAGE_FILE_HEADER -> NumberOfSections += 1;

	// TempNumber 的值等于 VirtualAddress + Misc.VirtualSize 与 PointerToRawData 之间较大的值
	DWORD TempNumber = 0x00000000;
	TempNumber = pPIMAGE_SECTION_HEADER ->VirtualAddress + \
		(pPIMAGE_SECTION_HEADER -> Misc.VirtualSize > pPIMAGE_SECTION_HEADER -> PointerToRawData ? \
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize : pPIMAGE_SECTION_HEADER -> PointerToRawData);

	// TempNumber 进行内存对齐
	if (!(TempNumber % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
	{
		pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage = TempNumber - (TempNumber % pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment) + pPIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
	}
	else
	{
		pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage = TempNumber;
	}

	return TRUE;
}

// ************************************************************************

BOOL CopyImageBufferToNewImageBufferEx(IN LPVOID pImageBuffer, OUT LPVOID pNewImageBuffer, IN UINT AlignNumber, IN UINT IsExistMisc, IN UINT CodeSize)
{
	/*
		此函数用于扩大节
		将 ImageBuffer 的数据复制到 NewImageBuffer 中
		校对所有需要修正的PE头里的值
		如果执行成功则返回TRUE 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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

	// 将 ImageBuffer 的内容复制到 NewImageBuffer 中
	memcpy(pNewImageBuffer, pImageBuffer, sizeof(CHAR) * pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);

	// 重新给结构体指针赋值为 NewImageBuffer 中的地址

	// DOS头结构体指针
	pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pNewImageBuffer;

	// NT头结构体指针
	pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 标准PE头结构体指针
	pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));

	// 可选PE头结构体指针
	pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 节表结构体指针
	pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 校准数据
	// 跳转到最后一个节
	pPIMAGE_SECTION_HEADER += (pPIMAGE_FILE_HEADER ->NumberOfSections - 1);

	// 名字可以不进行修改
	// 扩大节对齐前的尺寸 根据传入的 IsExistMisc 的值来写入
	if (!IsExistMisc)
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = 0;
	}
	else if (IsExistMisc == 1)
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = (pPIMAGE_SECTION_HEADER -> SizeOfRawData + AlignNumber - CodeSize);
	}
	else
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = (pPIMAGE_SECTION_HEADER -> SizeOfRawData + AlignNumber);
	}

	// VirtualAddress 的值不变
	// SizeOfRawData 的值改为 SizeOfRawData + AlignNumber
	pPIMAGE_SECTION_HEADER -> SizeOfRawData = pPIMAGE_SECTION_HEADER -> SizeOfRawData + AlignNumber;
	// PointerToRawData 的值不需要改
	// 扩大节的属性改为 可读 / 可写 / 可执行 / 包含可执行代码
    pPIMAGE_SECTION_HEADER -> Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE;

	// 更改 SizeOfImage 中的值
	pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage += AlignNumber;

	return TRUE;
}

// ************************************************************************

BOOL ChangeImageBufferData(IN LPVOID pImageBuffer, IN UINT IsExistMisc, IN UINT CodeSize)
{
	/*
		此函数用于合并节
		将 ImageBuffer 的数据全部修正
		如果执行成功则返回TRUE 失败则返回0
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pImageBuffer;

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

	// 判断是否只有一个节
	if (pPIMAGE_FILE_HEADER -> NumberOfSections <= 1)
	{
		// 如果只有一个节则直接返回NULL
		return NULL;
	}

	// 修改第一个节的数据
	// name 可以不用改
	// PointerToRawData 等于原来的值
	// VirtualAddress 等于原来的值
	// SizeOfImage 等于原来的值

	if (!IsExistMisc)
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = 0;
	}
	else if (IsExistMisc == 1)
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = (pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage - pPIMAGE_SECTION_HEADER -> VirtualAddress - CodeSize);
	}
	else
	{
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = (pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage - pPIMAGE_SECTION_HEADER -> VirtualAddress);
	}

	// SizeOfRawData 的值等于 SizeOfImage - VirtualAddress
	pPIMAGE_SECTION_HEADER -> SizeOfRawData = pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage - pPIMAGE_SECTION_HEADER -> VirtualAddress;

	// Characteristics 改为 包含其它节的全部属性
	DWORD TempCharacteristics = 0x00000000;
	int TempNumber;
	for (TempNumber = 0; TempNumber < pPIMAGE_FILE_HEADER -> NumberOfSections; TempNumber++)
	{
		TempCharacteristics |= pPIMAGE_SECTION_HEADER -> Characteristics;
		pPIMAGE_SECTION_HEADER++;
	}
	// 将节表指针还原
	pPIMAGE_SECTION_HEADER -= TempNumber;

	// 为节表 Characteristics 属性 赋值
	pPIMAGE_SECTION_HEADER -> Characteristics = TempCharacteristics;

	// 修改 NumberOfSections 的值为1
	pPIMAGE_FILE_HEADER -> NumberOfSections = 1;

	// 将 第一个节后面的 40 个字节全部置为0
	pPIMAGE_SECTION_HEADER++;
	for (int i = 0; i < IMAGE_SIZEOF_SECTION_HEADER; i++)
	{
		*((PUCHAR)pPIMAGE_SECTION_HEADER + i) = 0;
	}
	
	return TRUE;  
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

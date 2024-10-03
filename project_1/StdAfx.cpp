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
	if (!(TempCharacter & IMAGE_SCN_MEM_EXECUTE == IMAGE_SCN_MEM_EXECUTE))
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
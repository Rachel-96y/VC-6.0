#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;

typedef struct _IMAGE_DOS_HEADER 
{      
	// DOS头
    WORD   e_magic;                   
    WORD   e_cblp;                    
    WORD   e_cp;                       
    WORD   e_crlc;                 
    WORD   e_cparhdr;                   
    WORD   e_minalloc;                
    WORD   e_maxalloc;                 
    WORD   e_ss;                       
    WORD   e_sp;                      
    WORD   e_csum;                 
    WORD   e_ip;                        
    WORD   e_cs;                        
    WORD   e_lfarlc;                   
    WORD   e_ovno;                     
    WORD   e_res[4];                    
    WORD   e_oemid;                     
    WORD   e_oeminfo;                  
    WORD   e_res2[10];                  
    WORD   e_lfanew; 
	
} IMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER 
{
	// 标准PE头
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;

}IMAGE_FILE_HEADER;

typedef struct _IMAGE_OPTIONAL_HEADER 
{
	// 可选PE头
    //
    // Standard fields.
    //

    WORD    Magic;
    BYTE    MajorLinkerVersion;
    BYTE    MinorLinkerVersion;
    DWORD   SizeOfCode;
    DWORD   SizeOfInitializedData;
    DWORD   SizeOfUninitializedData;
    DWORD   AddressOfEntryPoint;
    DWORD   BaseOfCode;
    DWORD   BaseOfData;

    //
    // NT additional fields.
    //

    DWORD   ImageBase;
    DWORD   SectionAlignment;
    DWORD   FileAlignment;
    WORD    MajorOperatingSystemVersion;
    WORD    MinorOperatingSystemVersion;
    WORD    MajorImageVersion;
    WORD    MinorImageVersion;
    WORD    MajorSubsystemVersion;
    WORD    MinorSubsystemVersion;
    DWORD   Win32VersionValue;
    DWORD   SizeOfImage;
    DWORD   SizeOfHeaders;
    DWORD   CheckSum;
    WORD    Subsystem;
    WORD    DllCharacteristics;
    DWORD   SizeOfStackReserve;
    DWORD   SizeOfStackCommit;
    DWORD   SizeOfHeapReserve;
    DWORD   SizeOfHeapCommit;
    DWORD   LoaderFlags;
    DWORD   NumberOfRvaAndSizes;

}IMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_SECTION_HEADER 
{
	// 节表
    BYTE    Name[8];
    union {
            DWORD   PhysicalAddress;
            DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;

}IMAGE_SECTION_HEADER;


IMAGE_DOS_HEADER* pIMAGE_DOS_HEADER = NULL;  // 声明 dos 头结构体指针
IMAGE_FILE_HEADER* pIMAGE_FILE_HEADER = NULL;  // 声明标准 PE 头结构体指针
IMAGE_OPTIONAL_HEADER32* pIMAGE_OPTIONAL_HEADER32 = NULL;  // 声明可选 PE 头结构体指针
IMAGE_SECTION_HEADER* pIMAGE_SECTION_HEADER = NULL; // 声明节表结构体指针

_iobuf* pfileStream = NULL; // 声明文件管道指针
_iobuf* pNewfileStream = NULL; // 声明新的文件管道指针

void* pMemoryOfFileBuffer = NULL; // 声明 FileBuffer 堆空间指针 (用于释放空间)
char* pPrinterOfFileBuffer = NULL; // 声明 FileBuffer 指针 (用于计算)

void* pMemoryOfImageBuffer = NULL; // 声明 ImageBuffer 堆空间指针 (用于释放空间)
char* pPrinterOfImageBuffer = NULL; // 声明 ImageBuffer 指针(用于计算)

void* pMemoryOfNewFileBuffer = NULL; // 声明 ImageBuffer 堆空间指针 (用于释放空间)
char* pPrinterOfNewFileBuffer = NULL; // 声明 ImageBuffer 指针(用于计算)
							
DWORD sizeOfFile = 0; // 文件大小(字节)
			
DWORD ReadPEFile(char* FilePathOfPe)
{
	/* 
		ReadPEFile -> 将文件读取到缓冲区( FileBuffer )并为各个PE结构体指针赋值
		参数说明：
		FilePath -> 文件路径
		返回值 -> 如果发生异常则返回0，否则返回读取的文件大小
	*/

	pfileStream = fopen(FilePathOfPe, "rb");  // 打开文件
	if (pfileStream == NULL)
	{
		("文件打开失败");
		return 0;
	}
	fseek(pfileStream, 0, SEEK_END);  // 设置文件指针到末尾
	sizeOfFile = (DWORD)ftell(pfileStream);  // 求出文件所占字节数
	fseek(pfileStream, 0, SEEK_SET);  // 设置文件指针到开头
	pMemoryOfFileBuffer = malloc(sizeof(char) * sizeOfFile);  // 申请内存堆区
	if (pMemoryOfFileBuffer == NULL)
	{
		printf("内存申请失败");
		return 0;
	}
	memset(pMemoryOfFileBuffer, 0x00000000, sizeof(char) * sizeOfFile);  // 初始化 FileBuffer 内存的值为0
	DWORD sizeOfReadFile = fread(pMemoryOfFileBuffer, sizeof(char), sizeOfFile, pfileStream);  // 读取文件到堆空间
	fclose(pfileStream); // 关闭文件
	if (sizeOfReadFile != sizeOfFile)  
	{
		// 如果读取文件的字节数不正确
		printf("文件读取失败!"); 
		return 0;
	}

	// 为各个PE结构体指针赋值
	pPrinterOfFileBuffer = (char*)pMemoryOfFileBuffer; // 文件头地址赋值给 char* 指针 用于找到PE标志

	pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)(pMemoryOfFileBuffer);  // 为DOS头结构体指针赋值

	pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)(pPrinterOfFileBuffer + \
		(pIMAGE_DOS_HEADER -> e_lfanew + 4));// 为标准PE头结构体指针赋值

	pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)(pPrinterOfFileBuffer + \
	(pIMAGE_DOS_HEADER -> e_lfanew + 4 + 20)); // 为可选PE头结构体指针赋值

	pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)(pPrinterOfFileBuffer + \
	(pIMAGE_DOS_HEADER -> e_lfanew + 4 + 20 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader));  // 为节表结构体指针赋值

	if (pIMAGE_DOS_HEADER -> e_magic != 0x5A4D)
	{
		printf("不是有效的DOS_MZ标志!"); 
		return 0;
	}
	
	if (*(int*)(pPrinterOfFileBuffer + pIMAGE_DOS_HEADER -> e_lfanew) != 0x00004550) // 从头开始找
	{
		printf("不是有效的PE标识");
		return 0;
	}
	if (*(short int*)(pPrinterOfFileBuffer + pIMAGE_DOS_HEADER -> e_lfanew + 4 + 20) != 0x010B)
	{
		printf("暂时不支持非32位的程序");
		return 0;
	}
	return sizeOfReadFile;
}

DWORD CopyFileBufferToImageBuffer(char* pFileBuffer)
{
	/*
		CopyFileBufferToImageBuffer -> 将文件从 FileBuffer 复制到 ImageBuffe
		参数说明：
		pPrinterOfFileBuffer -> FileBuffer的指针
		返回值 -> 读取失败返回0  否则返回复制的字节数的总大小		
	*/
	BYTE SizeOfReturnNumbers = 0;
	pMemoryOfImageBuffer = malloc(pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage * sizeof(char));
	if (pMemoryOfImageBuffer == NULL)
	{
		// 读取失败
		printf("读取失败");
		return 0;
	}
	memset(pMemoryOfImageBuffer, 0x00000000, \
		pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage * sizeof(char));  // 初始化 ImageBuffe 内存的值为0
	pPrinterOfImageBuffer = (char*)pMemoryOfImageBuffer;
	// 首先复制SizeOfHeaders的大小 (所有头 + 节表 按照文件对齐后的大小)
	memcpy((BYTE*)pMemoryOfImageBuffer, (BYTE*)pFileBuffer, \
		sizeof(char) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);
	SizeOfReturnNumbers += sizeof(char) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders;

	// 获取节的数量
	// 循环获取每个节的 PointToRawData 和 VirtualAddress 的值
	// 循环调用 memcpy 来复制内存
	int NumberOfSum = 0;
	for (int i = 0; i < pIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		memcpy((BYTE*)(pPrinterOfImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress), \
			(BYTE*)(pFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData), \
			sizeof(char) * pIMAGE_SECTION_HEADER -> SizeOfRawData);
		SizeOfReturnNumbers += sizeof(char) * pIMAGE_SECTION_HEADER -> SizeOfRawData;
		pIMAGE_SECTION_HEADER++;
		NumberOfSum++;
	}
	pIMAGE_SECTION_HEADER -= NumberOfSum;  // 将节表结构体指针归位
	return SizeOfReturnNumbers;
}

DWORD CopyImageBufferToNewBuffer(char* pPrinterOfNewFileBuffer)
{
	// 将 ImageBuffer 还原为 NewFileBuffer
	// 需要 ImageBuffer 指针
	// 需要 NewFileBuffer 指针
	// 需要获取 ImageBuffer 的大小
	BYTE SizeOfReturnNumbers = 0;
	pMemoryOfNewFileBuffer = malloc(sizeof(char) * sizeOfFile);  // 开辟和文件大小相同的空间
	if (pMemoryOfNewFileBuffer == NULL)
	{
		return 0;
	}
	memset(pMemoryOfNewFileBuffer, 0x00000000, sizeof(char) * sizeOfFile);  // 初始化 ImageBuffe 内存的值为0
	pPrinterOfNewFileBuffer = (char*)pMemoryOfNewFileBuffer;
	// 按所有头加节表文件对齐后的大小进行复制
	memcpy((BYTE*)pMemoryOfNewFileBuffer, (BYTE*)pPrinterOfImageBuffer, \
		(DWORD)(sizeof(char) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders));
	// 开始复制节

	int NumberOfSum = 0;
	for (int i = 0; i < pIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		memcpy((BYTE*)(pPrinterOfNewFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData), \
			(BYTE*)(pPrinterOfImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress), \
			sizeof(char) * pIMAGE_SECTION_HEADER -> SizeOfRawData);
		SizeOfReturnNumbers += sizeof(char) * pIMAGE_SECTION_HEADER -> SizeOfRawData;
		pIMAGE_SECTION_HEADER++;
		NumberOfSum++;
	}
	pIMAGE_SECTION_HEADER -= NumberOfSum;  // 将节表结构体指针归位
	return SizeOfReturnNumbers;
}

void MemeryTOFile()
{
	pNewfileStream = fopen("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\NewNotepad.exe", "wb");
	// 测试: 直接存入文件
	fwrite(pMemoryOfNewFileBuffer, sizeof(char), sizeof(char) * sizeOfFile, pNewfileStream);
	fclose(pNewfileStream);
}

DWORD RvaToFileOffset(char* pFileBuffer,char* dwRva)
{
	// RVA转FOA的函数  dwRva: 内存中的偏移 pFileBuffer文件中的偏移

	// 将地址转为DWORD类型后减去首地址 得到偏移地址
	DWORD ResultOfRVA = (DWORD)dwRva;
	DWORD ResultOfImageBuffer = (DWORD)pPrinterOfImageBuffer;
	DWORD offsetRVA = ResultOfRVA - ResultOfImageBuffer; // 偏移地址
	int Sum = 0; // 最后的值表示 offsetRVA 在哪个节-1里
	// 不断进行大小比较,看这个值在哪个节里
	for (int i = 0; i < pIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		if (pIMAGE_SECTION_HEADER -> VirtualAddress	> offsetRVA && \
			++pIMAGE_SECTION_HEADER -> VirtualAddress < offsetRVA)
		{
			Sum = i + 1;
			break;
		}
		if (i + 1 == pIMAGE_FILE_HEADER -> NumberOfSections)
		{
			Sum = i + 1;
			break;
		}
	}
	pIMAGE_SECTION_HEADER -= Sum; // 归位节表结构体指针
	DWORD offsetFOA = (pIMAGE_SECTION_HEADER->PointerToRawData + Sum - 1) + offsetRVA;  // 得到FOA

	return offsetFOA;
}

/*

练习：	
						
1、实现如下功能

文件 -> FileBuffer -> ImageBuffer -> NewBuffer -> 存盘
		
2、编写一个函数，能够将RVA的值转换成FOA.(Relative Virtual Address -> File Ofseet Address)

*/	

int main(int argc, char* argv[])
{
	// 将文件读进缓冲区
	int isAvailable = ReadPEFile("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\notepad.exe");
	if (!isAvailable)
	{
		return 0;
	}
	// 将文件从 FileBuffer 复制到 ImageBuffer

	CopyFileBufferToImageBuffer(pPrinterOfFileBuffer);
	
	// 从 ImageBuffer 复制到 NewBuffer
	CopyImageBufferToNewBuffer(pPrinterOfNewFileBuffer);

	// 从 NewBuffer 写入文件
	MemeryTOFile();

	// 释放申请的堆空间
	free(pMemoryOfFileBuffer); // 释放 FileBuffer 堆空间
	free(pMemoryOfImageBuffer); // 释放 ImageBuffer 堆空间
	free(pMemoryOfNewFileBuffer); // 释放 NewFileBuffer 堆空间

	// 将剩余指针的值置为NULL
	
	pfileStream = NULL; // 文件管道指针 -> 0
	pNewfileStream = NULL; //新文件管道指针 -> 0


	pMemoryOfFileBuffer = NULL; // void* 类型的 FileBuffer 堆空间内存指针 -> 0
	pPrinterOfFileBuffer = NULL; // 由 pMemoryOfFileBuffer 赋值的char*类型的指针 -> 0

	pMemoryOfImageBuffer = NULL; // void* 类型的 ImageBuffer 堆空间内存指针 -> 0
	pPrinterOfImageBuffer = NULL; // 由 pMemoryOfImageBuffer 赋值的char*类型的指针 -> 0

	pMemoryOfNewFileBuffer = NULL; // void* 类型的 NewFileBuffer 堆空间指针 -> 0
	pPrinterOfNewFileBuffer = NULL; //由 pMemoryOfNewFileBuffer 赋值的char*类型的指针 -> 0
	

	pIMAGE_DOS_HEADER = NULL;  // DOS头结构体指针 -> 0
	pIMAGE_FILE_HEADER = NULL;  // 标准PE头结构体指针 -> 0
	pIMAGE_OPTIONAL_HEADER32 = NULL;  // 可选PE头结构体指针 -> 0
	pIMAGE_SECTION_HEADER = NULL; // 节表结构体指针 -> 0

	system("pause");
	return 0;
}
#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;

typedef struct _IMAGE_DOS_HEADER 
{      // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    WORD   e_lfanew;                    // File address of new exe header
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

/*

练习：

1、编写程序读取一个.exe文件，输出所有的PE头信息.

2、使用第三方的PE工具，对比信息，看是否一致

*/

int PrintOfPeHeaders(char* filePath)
{
	IMAGE_DOS_HEADER* pIMAGE_DOS_HEADER;  // dos头 ***
	IMAGE_FILE_HEADER* pIMAGE_FILE_HEADER;  // 标准PE头 ***
	IMAGE_OPTIONAL_HEADER32* pIMAGE_OPTIONAL_HEADER32;  // 可选PE头 ***

	_iobuf* pfileStream = fopen(filePath, "rb");  // 打开文件 ***
	if (pfileStream == NULL)
	{
		printf("文件打开失败");
		return NULL;
	}
	fseek(pfileStream, 0, SEEK_END);  // 设置文件指针到末尾
	DWORD sizeOfFile = (DWORD)ftell(pfileStream);  // 求出文件所占字节数
	fseek(pfileStream, 0, SEEK_SET);  // 设置文件指针到开头
	void* pMemory = malloc(sizeof(char)*sizeOfFile);  // 申请内存堆区 ***
	if (pMemory == NULL)
	{
		printf("内存申请失败");
		return NULL;
	}
	DWORD sizeOfReadFile = fread(pMemory, sizeof(char), sizeOfFile, pfileStream);  // 读取文件到堆空间
	fclose(pfileStream); // 关闭文件
	pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pMemory;  // 找到DOS头 64个字节 @@@
	if (sizeOfReadFile != sizeOfFile)  
	{
		// 如果读取文件的字节数不正确
		printf("文件读取失败!"); 
		return NULL;
	}
	if (pIMAGE_DOS_HEADER -> e_magic != 0x5A4D)
	{
		printf("不是有效的DOS_MZ标志!"); 
		return NULL;
	}
	char* pPrinterOfPE = (char*)pMemory; // 文件头地址赋值给char*指针 用于找到PE标志 ***
	if(*(int*)(pPrinterOfPE + pIMAGE_DOS_HEADER -> e_lfanew) != 0x00004550) // 从头开始找
	{
		printf("不是有效的PE标识");
		return NULL;
	}
	pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)\
		(pPrinterOfPE + (pIMAGE_DOS_HEADER -> e_lfanew) + 4);  // 找到标准PE头 20个字节  @@@
	
	pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)\
		(pPrinterOfPE + (pIMAGE_DOS_HEADER -> e_lfanew) + 4 + 20); // 找到扩展PE头 x86约224个字节 @@@
	
	/* -------------------------------  将关键的成员信息打印出来  ------------------------------- */
	// DOS 头
	printf("************************* DOS MZ 标识 ********************************\n");
	printf("%x\n", pIMAGE_DOS_HEADER -> e_magic);

	printf("************************* PE 偏移 ********************************\n");
	printf("%x\n", pIMAGE_DOS_HEADER -> e_lfanew);
	// -------------------------------------------------------------------------------------
	/* NT 头 */

	// PE 标识
	printf("************************* PE 标识 ********************************\n");
	printf("%x\n", *(int*)(pPrinterOfPE + pIMAGE_DOS_HEADER -> e_lfanew));

	// 标准 PE 头
	printf("************************* 程序运行的CPU型号 ********************************\n");
	printf("%x\n", pIMAGE_FILE_HEADER -> Machine);

	printf("************************* 文件中存在的节总数 ********************************\n");
	printf("%x\n", pIMAGE_FILE_HEADER -> NumberOfSections);

	printf("************************* 时间戳 ********************************\n");
	printf("%x\n", pIMAGE_FILE_HEADER -> TimeDateStamp);

	printf("************************* 可选PE头大小 ********************************\n");
	printf("%x\n", pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	printf("************************* 符号特征 ********************************\n");
	printf("%x\n", pIMAGE_FILE_HEADER -> Characteristics);

	// 可选 PE 头
	printf("************************* 说明文件类型(10B:32位下的PE文件|20B:64位下的PE文件) ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> Magic);

	printf("************************* 所有代码节的和 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfCode);

	printf("************************* 已初始化代码大小的和 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfInitializedData);

	printf("************************* 未初始化代码大小的和 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfUninitializedData);

	printf("************************* 程序入口点地址 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint);

	printf("************************* 代码开始的基址 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> BaseOfCode);

	printf("************************* 数据开始的基址 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> BaseOfData);

	printf("************************* 内存镜像基址 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> ImageBase);

	printf("************************* 内存对齐(节对齐) ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment);

	printf("************************* 文件对齐(节对齐) ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> FileAlignment);

	printf("************************* 内存中整个PE的映像大小 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);

	printf("************************* 所有头加节表按照文件对齐后的大小 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);

	printf("************************* 校验和 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> CheckSum);

	printf("************************* 初始化时保留的栈大小 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfStackReserve);

	printf("************************* 初始化时实际提交的栈大小 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfStackCommit);

	printf("************************* 初始化时保留的堆大小 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeapReserve);

	printf("************************* 初始化时实际提交的堆大小 ********************************\n");
	printf("%x\n", pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeapCommit);

	free(pMemory); // 释放堆空间

	// 初始化全部指针
	pMemory = NULL; // 堆空间内存指针 -> 0
	pPrinterOfPE = NULL; // 由pMemory赋值的char*类型的指针 -> 0
	pfileStream = NULL; // 文件管道指针 -> 0
	pIMAGE_DOS_HEADER = NULL;  // DOS头结构体指针 -> 0
	pIMAGE_FILE_HEADER = NULL;  // 标准PE头结构体指针 -> 0
	pIMAGE_OPTIONAL_HEADER32 = NULL;  // 可选PE头结构体指针 -> 0

	return 0;
}

int main(int argc, char* argv[])
{
	// 输出所有的PE头信息
	printf("请输入文件路径: (如: C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\notepad.exe)\n");
	char PathOfValue[200]; 
	scanf("%s", PathOfValue);
	char* result = PathOfValue;
	PrintOfPeHeaders(result);
	system("pause");
	return 0;
}
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

/*

练习：							
							
1、编写程序打印节表中的信息.				  	
							
2、根据节表中的信息，到文件中找到所有的节，观察节的开始位置与大小是否							
与节表中的描述一致(使用notepad.exe来练习).							

*/

int PrintOfPeHeaders(char* filePath)
{
	IMAGE_DOS_HEADER* pIMAGE_DOS_HEADER;  // 声明dos头指针 ***
	IMAGE_FILE_HEADER* pIMAGE_FILE_HEADER;  // 声明标准PE头指针 ***
	IMAGE_OPTIONAL_HEADER32* pIMAGE_OPTIONAL_HEADER32;  // 声明可选PE头指针 ***
	IMAGE_SECTION_HEADER* pIMAGE_SECTION_HEADER; // 声明节表指针 *** 

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
		(pPrinterOfPE + (pIMAGE_DOS_HEADER -> e_lfanew) + 4);  // 找到标准PE头 20个字节 赋值 @@@ 
	
	pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)\
		(pPrinterOfPE + (pIMAGE_DOS_HEADER -> e_lfanew) + 4 + 20); // 找到扩展PE头 x86 约224个字节 赋值 @@@
		
	pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)\
		(pPrinterOfPE + (pIMAGE_DOS_HEADER -> e_lfanew) \
		+ 4 + 20 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);// 找到节表 每个节表40个字节  赋值 @@@

	/* -------------------------------  将节表的成员信息打印出来  ------------------------------- */
	// 同于测试的文件路径: C:\\Windows\\System32\\notepad.exe
	
	BYTE NameArr[9];  // 此数组用于正确打印节表中成员Name的值 (8个字节)
	NameArr[8] = 0x00;  // 将最后一个值赋值为0方便printf函数进行%s打印
	for (int i = 0; i < pIMAGE_FILE_HEADER -> NumberOfSections; i++)  // NumberOfSections 指明了有多少个节
	{
		for (int j = 0; j < 8; j++)
		{
			NameArr[j] = pIMAGE_SECTION_HEADER -> Name[j];  // 为NameArr数组赋值
		}
		printf("\n第%d个节的信息:\n", i + 1);
		printf("Name的值是: %s\n", NameArr);
		printf("MISC的值是: %x\n", pIMAGE_SECTION_HEADER -> Misc);
		printf("VirtualAddress的值是: %x\n", pIMAGE_SECTION_HEADER -> VirtualAddress);
		printf("SizeOfRawData的值是: %x\n", pIMAGE_SECTION_HEADER -> SizeOfRawData);
		printf("PointerToRawData的值是: %x\n", pIMAGE_SECTION_HEADER -> PointerToRawData);
		printf("PointerToRelocations的值是: %x\n", pIMAGE_SECTION_HEADER -> PointerToRelocations);
		printf("PointerToLinenumbers的值是: %x\n", pIMAGE_SECTION_HEADER -> PointerToLinenumbers);
		printf("NumberOfRelocations的值是: %x\n", pIMAGE_SECTION_HEADER -> NumberOfRelocations);
		printf("NumberOfLinenumbers的值是: %x\n", pIMAGE_SECTION_HEADER -> NumberOfLinenumbers);
		printf("Characteristics的值是: %x\n", pIMAGE_SECTION_HEADER -> Characteristics);

		pIMAGE_SECTION_HEADER++; // 跳过整个结构体 找到下一个结构体
	}
	printf("\n");
	free(pMemory); // 释放堆空间
	// 初始化全部指针
	pMemory = NULL; // void*类型的堆空间内存指针 -> 0
	pPrinterOfPE = NULL; // 由pMemory赋值的char*类型的指针 -> 0
	pfileStream = NULL; // 文件管道指针 -> 0
	pIMAGE_DOS_HEADER = NULL;  // DOS头结构体指针 -> 0
	pIMAGE_FILE_HEADER = NULL;  // 标准PE头结构体指针 -> 0
	pIMAGE_OPTIONAL_HEADER32 = NULL;  // 可选PE头结构体指针 -> 0
	pIMAGE_SECTION_HEADER = NULL; // 节表指针 -> 0

	return 0;
}

int main(int argc, char* argv[])
{
	// 输出所有的PE头信息
	printf("请输入文件路径: ( 如: C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\notepad.exe )\n");
	char PathOfValue[200]; 
	scanf("%s", PathOfValue);
	char* result = PathOfValue;
	PrintOfPeHeaders(result);
	system("pause");
	return 0;
}
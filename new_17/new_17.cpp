/*
本节练习：
1、找出所有DOS头数据，并统计DOS头大小.
2、找出所有标准PE头数据，并统计标准PE头大小.
3、找出所有可选PE头数据，并统计可选PE头大小.
4、背出所有重要的成员在哪个结构体中
*/

#include "stdafx.h"

/*
 练习所使用的.exe文件路径:   
"D:\study\C_X86\HD\汇编\6(标志寄存器)\2015-01-19\CRACKME.EXE"
*/

/*
// DOS头
typedef struct _IMAGE_DOS_HEADER {      
    WORD   e_magic;						*		//	5A 4D
    WORD   e_cblp;								//	00 50
    WORD   e_cp;								//	00 02
    WORD   e_crlc;								//	00 00
    WORD   e_cparhdr;							//	00 04
    WORD   e_minalloc;							//	00 0F
    WORD   e_maxalloc;							//	FF FF
    WORD   e_ss;								//	00 00
    WORD   e_sp;								//	00 B8
    WORD   e_csum;								//	00 00
    WORD   e_ip;								//  00 00
    WORD   e_cs;								//	00 00
    WORD   e_lfarlc;							//	00 40
    WORD   e_ovno;								//  00 1A
    WORD   e_res[4];							//	00 00 00 00 00 00 00 00
    WORD   e_oemid;								//	00 00
    WORD   e_oeminfo;							//	00 00
    WORD   e_res2[10];							//	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    LONG   e_lfanew;					*		//	00 00 01 00
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;		//

DOS头大小: 64 个字节

// NT头
typedef struct _IMAGE_NT_HEADERS {
    DWORD Signature;					*		//	00 00 45 50
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

// 标准PE头
typedef struct _IMAGE_FILE_HEADER {
    WORD    Machine;					*		//	01 4C
    WORD    NumberOfSections;			*		//	00 06
    DWORD   TimeDateStamp;				*		//	0A D9 24 29
    DWORD   PointerToSymbolTable;				//	00 00 00 00
    DWORD   NumberOfSymbols;					//	00 00 00 00
    WORD    SizeOfOptionalHeader;		*		//	00 E0
    WORD    Characteristics;			*		//	81 8E
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

标准PE头大小: 20 个字节

// 可选PE头
typedef struct _IMAGE_OPTIONAL_HEADER {
    //
    // Standard fields.
    //

    WORD    Magic;						*		//	01 0B
    BYTE    MajorLinkerVersion;					//	02
    BYTE    MinorLinkerVersion;					//	19
    DWORD   SizeOfCode;					*		//	00 00 06 00
    DWORD   SizeOfInitializedData;		*		//	00 00 22 00
    DWORD   SizeOfUninitializedData;	*		//	00 00 00 00
    DWORD   AddressOfEntryPoint;		*		//	00 00 10 00
    DWORD   BaseOfCode;					*		//	00 00 10 00
    DWORD   BaseOfData;					*		//	00 00 20 00

    //
    // NT additional fields.
    //

    DWORD   ImageBase;					*		//	00 40 00 00
    DWORD   SectionAlignment;			*		//	00 00 10 00
    DWORD   FileAlignment;				*		//	00 00 02 00
    WORD    MajorOperatingSystemVersion;		//	00 01
    WORD    MinorOperatingSystemVersion;		//	00 00
    WORD    MajorImageVersion;					//	00 00
    WORD    MinorImageVersion;					//	00 00
    WORD    MajorSubsystemVersion;				//	00 03
    WORD    MinorSubsystemVersion;				//	00 0A
    DWORD   Win32VersionValue;					//	00 00 00 00
    DWORD   SizeOfImage;				*		//	00 00 80 00
    DWORD   SizeOfHeaders;				*		//	00 00 04 00
    DWORD   CheckSum;					*		//	00 00 00 00
    WORD    Subsystem;							//	00 02
    WORD    DllCharacteristics;					//	00 00
    DWORD   SizeOfStackReserve;			*		//	00 10 00 00
    DWORD   SizeOfStackCommit;			*		//	00 00 20 00
    DWORD   SizeOfHeapReserve;			*		//	00 10 00 00
    DWORD   SizeOfHeapCommit;			*		//	00 00 10 00
    DWORD   LoaderFlags;						//	00 00 00 00
    DWORD   NumberOfRvaAndSizes;				//	00 00 00 10
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

可选PE头大小: 96 个字节 + 128个字节 总共224个字节

*/
int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	return 0;
}
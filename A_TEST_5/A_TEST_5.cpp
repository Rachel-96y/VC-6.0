// A_TEST_5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

BOOL PrintImportDescriptor(LPVOID pFileBuffer)
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

	/*
	找单个函数名称
	LPSTR DLL_NAME_PTR = (LPSTR)(ChangeRvaToFoa(pPIMAGE_IMPORT_DESCRIPTOR -> Name, pFileBuffer) + (DWORD)pFileBuffer);
	printf("%s\n", DLL_NAME_PTR);

	DWORD pPIMAGE_THUNK_DATA32 = (DWORD)(ChangeRvaToFoa(pPIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk, pFileBuffer) + (DWORD)pFileBuffer);
	printf("%x\n", pPIMAGE_THUNK_DATA32);
	PIMAGE_IMPORT_BY_NAME pPIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa(*(LPDWORD)pPIMAGE_THUNK_DATA32, pFileBuffer) + (DWORD)pFileBuffer);
	printf("%s\n", pPIMAGE_IMPORT_BY_NAME -> Name);
	*/

	// 如果导入表结构体指针有值则开始打印
	if (!pPIMAGE_IMPORT_DESCRIPTOR)
	{
		return NULL;
	}

	do
	{
		// 判断是否导入表外层结构结束 结束则跳出
		if (!(pPIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pPIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pPIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pPIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pPIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;

		// 遍历需要导入的全部的 DLL 循环打印 _IMAGE_IMPORT_DESCRIPTOR 导入表信息
		printf("---------------------------------------------------------------------------\n");
		printf("OriginalFirstThunk: %x\n", pPIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk);
		printf("TimeDateStamp: %x\n", pPIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp);
		printf("ForwarderChain: %x\n", pPIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain);
		printf("指向将要使用的DLL的名字的地址: %x\n", pPIMAGE_IMPORT_DESCRIPTOR -> Name);
		printf("FirstThunk: %x\n", pPIMAGE_IMPORT_DESCRIPTOR -> FirstThunk);

		// 打印模块名称
		LPSTR DLL_NAME_PTR = (LPSTR)(ChangeRvaToFoa(pPIMAGE_IMPORT_DESCRIPTOR -> Name, pFileBuffer) + (DWORD)pFileBuffer);
		printf("DLL名称: %s\n", DLL_NAME_PTR);
		printf("\n");

		// 打印深层结构

		// 打印 INT 表
		// 找到 PIMAGE_THUNK_DATA32 的 FOA (4字节)
		DWORD pINT_PIMAGE_THUNK_DATA32 = (DWORD)(ChangeRvaToFoa(pPIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk, pFileBuffer) + (DWORD)pFileBuffer);
		while(*(LPDWORD)pINT_PIMAGE_THUNK_DATA32 != 0)
		{
			// 值为0则循环结束
			
			// 判断最高位是否为1
			if (*(LPDWORD)pINT_PIMAGE_THUNK_DATA32 & IMAGE_ORDINAL_FLAG32)
			{
				// 如果为1 那么除去最高位的值 就是函数的导出序号
				DWORD Original = *(LPDWORD)pINT_PIMAGE_THUNK_DATA32 & ~IMAGE_ORDINAL_FLAG32;
				printf("函数序号(INT): %x\n", Original);
			}
			// 如果不是，那么这个值是一个RVA 指向 IMAGE_IMPORT_BY_NAME 
			else
			{	// 打印 INT 表
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa(*(LPDWORD)pINT_PIMAGE_THUNK_DATA32, pFileBuffer) + (DWORD)pFileBuffer);
				printf("函数名称(INT): %s\n", pINT_PIMAGE_IMPORT_BY_NAME -> Name);
				
			}

			// 找到下一个地址
			pINT_PIMAGE_THUNK_DATA32 += 4;
		}

		// 打印 IAT 表
		// 找到 PIMAGE_THUNK_DATA32 的 FOA (4字节)
		DWORD pPIMAGE_THUNK_DATA32 = (DWORD)(ChangeRvaToFoa(pPIMAGE_IMPORT_DESCRIPTOR -> FirstThunk, pFileBuffer) + (DWORD)pFileBuffer);
		
		printf("\n\n");

		// 定义 IAT 表结构体指针 FirstThunk 指向 IAT 表
		while(*(LPDWORD)pPIMAGE_THUNK_DATA32 != 0)
		{
			// 值为0则循环结束

			// 判断此DLL是否被绑定 如果被绑定直接打印函数地址
			if (pPIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp)
			{
				printf("函数地址(Bound): %x\n", *(LPDWORD)pPIMAGE_THUNK_DATA32);

				// 找到下一个地址
				pPIMAGE_THUNK_DATA32 += 4;
				continue;
			}

			// 判断最高位是否为1
			if (*(LPDWORD)pPIMAGE_THUNK_DATA32 & IMAGE_ORDINAL_FLAG32)
			{
				// 如果为1 那么除去最高位的值 就是函数的导出序号
				DWORD Original = *(LPDWORD)pPIMAGE_THUNK_DATA32 & ~IMAGE_ORDINAL_FLAG32;
				printf("函数序号(IAT): %x\n", Original);
			}

			// 如果不是，那么这个值是一个RVA 指向 IMAGE_IMPORT_BY_NAME 
			else
			{
				PIMAGE_IMPORT_BY_NAME pPIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa(*(LPDWORD)pPIMAGE_THUNK_DATA32, pFileBuffer) + (DWORD)pFileBuffer);
				printf("函数名称(IAT): %s\n", pPIMAGE_IMPORT_BY_NAME -> Name);
				
			}

			// 找到下一个地址
			pPIMAGE_THUNK_DATA32 += 4;
		}

		// 找到下一个模块
		pPIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);

	return NULL;

}



int main(int argc, char* argv[])
{
	/*
		实现：							
		1、使用OD打开一个发布版的exe程序，定位到某个DLL的API									
		2、在没有加载的EXE中找到这个位置，观察加载前后的区别					

		练习：
		1、打印 ipmsg.exe 导入表的全部信息
		2、打印 notepad.exe 导入表的全部信息
		3、观察两个 EXE 的 IAT 表在加载前有什么区别？
	*/

	// PATH NEW_PATH
	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(NEW_PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD SIZE_OF_FILE_BUFFER = ReadPeFileToFileBuffer(NEW_PATH, pFileBuffer);

	// 打印导入表
	PrintImportDescriptor(pFileBuffer);

	getchar();

	return 0;
}
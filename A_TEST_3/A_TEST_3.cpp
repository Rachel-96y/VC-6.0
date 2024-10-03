// A_TEST_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


VOID TestPrintPeFormat(IN LPVOID pMemory, IN UINT Size)
{
	/* 
		此函数用于打印PE文件的数据 
		以十六进制的格式显示
		需要传入 Size 显示的大小 及 pMemory 指针
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pMemory;

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
		return;
	}

	// 判断PE标识是否有效
	if (*(PDWORD)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew) != IMAGE_NT_SIGNATURE)
	{
		printf("这不是一个有效的PE文件: 错误的PE标志");
		return;
	}

	for (unsigned int i = 0; i < Size; i++)
	{

		if (i != 0 && i % 0x10 == 0)
		{
			printf("\n");
		}

		switch(*((char*)pMemory + i))
		{
			case 0x00:
				printf("00    ");
				break;
			case 0x01:
				printf("01    ");
				break;
			case 0x02:
				printf("02    ");
				break;
			case 0x03:
				printf("03    ");
				break;
			case 0x04:
				printf("04    ");
				break;
			case 0x05:
				printf("05    ");
				break;
			case 0x06:
				printf("06    ");
				break;
			case 0x07:
				printf("07    ");
				break;
			case 0x08:
				printf("08    ");
				break;
			case 0x09:
				printf("09    ");
				break;
			case 0x0A:
				printf("0a    ");
				break;
			case 0x0B:
				printf("0b    ");
				break;
			case 0x0C:
				printf("0c    ");
				break;
			case 0x0D:
				printf("0d    ");
				break;
			case 0x0E:
				printf("0e    ");
				break;
			case 0x0F:
				printf("0f    ");
				break;
			default:
				printf("%x    ", (unsigned char)*((char*)pMemory + i));
		}
	}
	
}


VOID TestDll()
{
	/* 此函数用于测试被修改后的DLL能不能正常执行 */

	HMODULE hmodule = LoadLibrary(NEW_PATH);

	int (__stdcall *pPlus) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Plus@8");
	int (__stdcall *pSub) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Sub@8");
	int (__stdcall *pMul) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Mul@8");
	int (__stdcall *pDiv) (int, int) = (int (__stdcall *) (int, int))GetProcAddress(hmodule, "_Div@8");

	int ret1 = pPlus(4, 3);
	int ret2 = pSub(7, 2);
	int ret3 = pMul(6, 4);
	int ret4 = pDiv(9, 3);

	printf("%d\n%d\n%d\n%d", ret1, ret2, ret3, ret4);
	
	return;
}


DWORD SizeOfNewDll(IN LPVOID pFileBuffer)
{
	/* 计算新生成的DLL需要多少空间 */

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

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pPIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	
	// 计算出在 FileBuffer 中导出表的实际偏移地址(FOA) 加上 FileBuffer 的值 赋值给导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);
	
	// 计算出重定位表的 FOA 并加上 pFileBuffer 的值 赋值给重定位表结构体指针
	PIMAGE_BASE_RELOCATION pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa((pPIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);
	
	// 先计算导出表及其相关数据的大小
	// 将结果放入 SizeOf_IMAGE_EXPORT_DIRECTORY 中
	// 全部名字所占的字节数 先存放在 NumberOfAllNames 中
	DWORD NumberOfAllNames = 0;
	for (UINT i = 0; i < pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames; i++)
	{
		// 计算出 AddressOfNames 所有的偏移地址 需要加上 pFileBuffer
		LPDWORD TEMP_NUMBER = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames, pFileBuffer) + i * 4 + (DWORD)pFileBuffer);

		// 计算出这些偏移地址中的值的 Rva + pFileBuffer 得到的值 并将其转换为 LPCSTR 类型 使用 strlen 计算出地址中的字符串长度
		NumberOfAllNames += strlen((LPCSTR)(ChangeRvaToFoa(*TEMP_NUMBER, pFileBuffer) + (DWORD)pFileBuffer));
	}

	// 导出表 SizeOf_IMAGE_EXPORT_DIRECTORY 的总大小 = AddressOfFunctions + AddressOfNames + AddressOfNameOrdinals + IMAGE_EXPORT_DIRECTORY + NumberOfAllNames
	DWORD SizeOf_IMAGE_EXPORT_DIRECTORY = pPIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions * sizeof(UINT) + pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames * sizeof(UINT) + \
	pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames * sizeof(USHORT) + sizeof(IMAGE_EXPORT_DIRECTORY) + NumberOfAllNames;
	
	// 再计算重定位表及其相关数据的大小
	// 将结果放入 SizeOf_IMAGE_BASE_RELOCATION 中
	DWORD SizeOf_IMAGE_BASE_RELOCATION = 0;

	// 判断是否第一个块有值
	if (!(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress))
		// 如果重定位表没有值则 导出表占用的大小就是需要申请的空间的总大小
		return SizeOf_IMAGE_EXPORT_DIRECTORY;

	// 重定位表的总大小 = 每个块的大小相加
	do
	{
		// 如果 VirtualAddress 或者 SizeOfBlock 的值为0 则重定位表结束
		 SizeOf_IMAGE_BASE_RELOCATION += pPIMAGE_BASE_RELOCATION -> SizeOfBlock;

		// 跳过一个块的全部地址 注意这里不需要加 IMAGE_SIZEOF_BASE_RELOCATION
		pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pPIMAGE_BASE_RELOCATION + pPIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}
	while(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress);

	// 总大小 = SizeOf_IMAGE_EXPORT_DIRECTORY + SizeOf_IMAGE_BASE_RELOCATION
	return SizeOf_IMAGE_EXPORT_DIRECTORY + SizeOf_IMAGE_BASE_RELOCATION;
}


LPVOID CopyFileBufferToNewFileBuffer(IN LPVOID pFileBuffer, IN UINT SizeOfNewFileBuffer, IN UINT SizeOfOldFileBuffer)
{
	/* 申请空间并复制数据 */

	// SizeOfNewFileBuffer 及为整个需要申请的空间大小 (新增节后的大小)
	LPVOID pNewFileBuffer = malloc(sizeof(CHAR) * SizeOfNewFileBuffer);
	if (pNewFileBuffer == NULL)
	{
		// 空间申请失败则直接返回
		return NULL;
	}

	memset(pNewFileBuffer, 0x00000000, SizeOfNewFileBuffer);

	// 将 pFileBuffer 的内容复制到 pNewFileBuffer 中
	memcpy(pNewFileBuffer, pFileBuffer, sizeof(CHAR) * SizeOfOldFileBuffer);

	return pNewFileBuffer;
}


DWORD AddNewSection(IN LPVOID pNewFileBuffer, IN UINT SizeOfNewDll_FileAlignment, IN UINT SizeOfNewDll_ImageAlignment, IN UINT SizeOfOldFileBuffer)
{
	/* 此函数用于新增一个节并修正数据 */

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pNewFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 新增一个节表 (正规的做法是先判断是否有空间新增一个节表 这里省略)
	// 找到需要新增的节表的位置
	pPIMAGE_SECTION_HEADER += pPIMAGE_FILE_HEADER -> NumberOfSections;

	// 更改此节表的数据

	// 节名字
	pPIMAGE_SECTION_HEADER -> Name[0] = '.';
	pPIMAGE_SECTION_HEADER -> Name[1] = 'M';
	pPIMAGE_SECTION_HEADER -> Name[2] = 'y';
	pPIMAGE_SECTION_HEADER -> Name[3] = 'C';
	pPIMAGE_SECTION_HEADER -> Name[4] = 'o';
	pPIMAGE_SECTION_HEADER -> Name[5] = 'd';
	pPIMAGE_SECTION_HEADER -> Name[6] = 'e';
	pPIMAGE_SECTION_HEADER -> Name[7] = '\0';


	// VirtualSize 的大小等于新增的节的大小 这个值可以不准确
	pPIMAGE_SECTION_HEADER -> Misc.VirtualSize = SizeOfNewDll_FileAlignment;

	// NewVirtualAddress 用于存储最终的 VirtualAddress 的值
	DWORD NewVirtualAddress = 0;
	// 转到上一个节
	pPIMAGE_SECTION_HEADER--;
	// VirtualAddress 的起始位置是 上一个节的 VirtualAddress + 上一个节的 Misc.VirtualSize 和 SizeOfRawData 中的最大值
	NewVirtualAddress = pPIMAGE_SECTION_HEADER -> VirtualAddress + \
		(pPIMAGE_SECTION_HEADER -> Misc.VirtualSize > pPIMAGE_SECTION_HEADER -> SizeOfRawData ? \
		pPIMAGE_SECTION_HEADER -> Misc.VirtualSize : pPIMAGE_SECTION_HEADER -> SizeOfRawData);
	// 转到当前节
	pPIMAGE_SECTION_HEADER++;
	// 为当前节的 VirtualAddress 赋值
	pPIMAGE_SECTION_HEADER -> VirtualAddress = NewVirtualAddress;

	// SizeOfRawData 的大小等于新增的节的大小 也等于 VirtualSize 的大小
	pPIMAGE_SECTION_HEADER -> SizeOfRawData = SizeOfNewDll_FileAlignment;

	// 新增节的 PointerToRawData 的起始位置等于上一个节的 PointerToRawData + 上一个节的 SizeOfRawData
	pPIMAGE_SECTION_HEADER--;
	DWORD TempPointerToRawData = pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> SizeOfRawData;
	pPIMAGE_SECTION_HEADER++;
	pPIMAGE_SECTION_HEADER -> PointerToRawData = TempPointerToRawData;

	// 将节表结构体指针重新指向第一个节 遍历所有节的属性 将新增的节的属性改为其它节的属性的和
	pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	DWORD AllSectionCharacters = 0;
	
	// 更改节属性
	for (int i = 0; i < pPIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		// 将所有节的属性存储在 AllSectionCharacter 中
		AllSectionCharacters |= pPIMAGE_SECTION_HEADER++ -> Characteristics;
	}
	// 将节表结构体指针指向新增节并赋值
	++pPIMAGE_SECTION_HEADER -> Characteristics = AllSectionCharacters;

	// 将标准 PE 头中 NumberOfSection 的值加1
	pPIMAGE_FILE_HEADER -> NumberOfSections += 1;

	// 更改可选 PE 头中 SizeOfImage 的值 为新增的节内存对齐后的大小 + 曾经的 SizeOfImage 的大小
	pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage += SizeOfNewDll_ImageAlignment;

	return pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
}


LPVOID ChangeDLL_IMAGE_EXPORT_DIRECTORY(IN LPVOID pNewFileBuffer)
{
	/* 此函数用于移动导出表至新增的节里并修正定位数据 */

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pNewFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pPIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	
	// 计算出在 FileBuffer 中导出表的实际偏移地址(FOA) 加上 FileBuffer 的值 赋值给导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pNewFileBuffer) + (DWORD)pNewFileBuffer);
	

	// ------ 移动 AddressOfFunctions 中所有地址里的值 ------


	// 新增的节的开始位置 FileBuffer 的起始位置 + 最后一个节的 PointerToRawData + 最后一个节的 SizeOfRawData

	// 转到新增节的节表 这里减 1 是因为已经新增了一个节 此操作会覆盖掉最后一个节数据后的值 (需要确保那些值是没用的)
	pPIMAGE_SECTION_HEADER += (pPIMAGE_FILE_HEADER -> NumberOfSections - 1);

	LPVOID PointerToNewSection = (LPVOID)(pPIMAGE_SECTION_HEADER -> PointerToRawData + (DWORD)pNewFileBuffer);

	pPIMAGE_SECTION_HEADER -= pPIMAGE_FILE_HEADER -> NumberOfSections;

	// NewAddressOfFunctions 存储新的 AddressOfFunctions 的值 FOA
	UINT NewAddressOfFunctions = (DWORD)PointerToNewSection - (DWORD)pNewFileBuffer;

	//  SizeOfFunctions 要复制的大小是 NumberOfFunctions * 4
	UINT SizeOfFunctions = pPIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions * sizeof(UINT);

	// 从 AddressOfFunctions 的起始位置复制到 PointerToNewSection 的位置
	memcpy(PointerToNewSection, (LPVOID)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions, pNewFileBuffer) + (DWORD)pNewFileBuffer), SizeOfFunctions);
	
	// 计算当前指针位置 (已复制 AddressOfFunctions)
	PointerToNewSection = (LPVOID)((DWORD)PointerToNewSection + SizeOfFunctions);


	// ------ 移动 AddressOfNames 中所有地址里的值 ------
	

	// NewAddressOfNames 存储新的 AddressOfNames 的开始地址 FOA
	UINT NewAddressOfNames = (DWORD)PointerToNewSection - (DWORD)pNewFileBuffer;
	
	// SizeOfNames 要复制的大小是 NumberOfNames * 4
	UINT SizeOfNames = pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames * sizeof(UINT);
	
	// 开始拷贝数据 
	memcpy(PointerToNewSection, (LPVOID)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames, pNewFileBuffer) + (DWORD)pNewFileBuffer), SizeOfNames);
	
	// 计算当前指针位置 (已复制 AddressOfFunctions 及 AddressOfNames)
	PointerToNewSection = (LPVOID)((DWORD)PointerToNewSection + SizeOfNames);


	// ------ 移动 AddressOfNameOrdinals 中所有地址里的值 ------


	// NewAddressOfNameOrdinals 存储新的 AddressOfNameOrdinals 的开始地址 FOA
	UINT NewAddressOfNameOrdinals = (DWORD)PointerToNewSection - (DWORD)pNewFileBuffer;

	// SizeOfNameOrdinals 要复制的大小是 NumberOfNames * 2
	UINT SizeOfNameOrdinals = pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames * sizeof(SHORT);
	
	// 在新增的节中的开始位置是目前的 PointerToNewSection
	memcpy(PointerToNewSection, (LPVOID)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals, pNewFileBuffer) + (DWORD)pNewFileBuffer), SizeOfNameOrdinals);
	
	// 计算当前指针位置 (已复制 AddressOfFunctions 及 AddressOfNames 及 AddressOfNameOrdinals)
	PointerToNewSection = (LPVOID)((DWORD)PointerToNewSection + SizeOfNameOrdinals);


	// ------ 移动 AddressOfNames 中所有地址里的值所指向的名字 ------
	// 注意 移动名字的时候需要每移动一个 将名字表里的


	// 得到 NewAddressOfFunctionNames 在文件中的地址 FOA + pNewFileBuffer (新增节中的名字表的首地址)
	DWORD NewAddressOfFunctionNames = NewAddressOfNames + (DWORD)pNewFileBuffer;

	// AddressOfNames 的首地址 (原名字表的首地址)
	LPDWORD AddressOfNamesFoa = (LPDWORD)(ChangeRvaToFoa(pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames, pNewFileBuffer) + (DWORD)pNewFileBuffer);

	for(UINT i = 0; i < pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames; i++)
	{
		// 名字个数 = pPIMAGE_EXPORT_DIRECTORY -> NumberOfNames

		// 取出 NumberOfNames 中的每个值并转为 FOA 后 加上 pNewFileBuffer(基地址) 得到字符串的起始地址
		LPSTR pNamesFoa = (LPSTR)(ChangeRvaToFoa(*(AddressOfNamesFoa + i), pNewFileBuffer) + (DWORD)pNewFileBuffer);

		// 获取名字长度 用于偏移指针
		UINT NameLength = strlen(pNamesFoa);

		// 复制名字到新增节中
		strcpy((LPSTR)PointerToNewSection, (LPCSTR)pNamesFoa);

		// 找到当前指针位置 将这个名字的新的首地址写入名字地址表 需要转为RVA
		*(LPDWORD)NewAddressOfFunctionNames = ChangeFoaToRva((DWORD)PointerToNewSection - (DWORD)pNewFileBuffer, pNewFileBuffer);

		// 跳过一个名字的长度
		PointerToNewSection = LPVOID((DWORD)PointerToNewSection + NameLength + 1);

		// 将名字地址表指针偏移4字节的长度
		NewAddressOfFunctionNames += sizeof(UINT);

	}


	// ------ 移动 导出表 _IMAGE_EXPORT_DIRECTORY 的所有成员 ------
	// PointerToNewSection 为当前指针 (指向最后一个名字末尾)
	// 找到 _IMAGE_EXPORT_DIRECTORY 的起始位置为: pPIMAGE_EXPORT_DIRECTORY 写入到 PointerToNewSection 的位置 写 40 个字节的大小
	memcpy(PointerToNewSection, pPIMAGE_EXPORT_DIRECTORY, sizeof(_IMAGE_EXPORT_DIRECTORY));

	// 更改 指向 _IMAGE_EXPORT_DIRECTORY 的值 (数据目录中导出表的 RVA ) 导出表为第一个 pPIMAGE_DATA_DIRECTORY 结构
	pPIMAGE_DATA_DIRECTORY -> VirtualAddress = ChangeFoaToRva((DWORD)PointerToNewSection - (DWORD)pNewFileBuffer, pNewFileBuffer);

	// 重新给 pPIMAGE_EXPORT_DIRECTORY 赋值
	pPIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa(pPIMAGE_DATA_DIRECTORY -> VirtualAddress, pNewFileBuffer) + (DWORD)pNewFileBuffer);

	// 更改指向 注意 NewAddressOfFunctions NewAddressOfNameOrdinals NewAddressOfNames 里的值需要转为 RVA
	// AddressOfFunctions
	pPIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions = ChangeFoaToRva(NewAddressOfFunctions, pNewFileBuffer);

	// AddressOfNames
	pPIMAGE_EXPORT_DIRECTORY -> AddressOfNames = ChangeFoaToRva(NewAddressOfNames, pNewFileBuffer);

	// AddressOfNameOrdinals
	pPIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals = ChangeFoaToRva(NewAddressOfNameOrdinals, pNewFileBuffer);

	// 计算当前指针位置 并返回 (已加上了 pNewFileBuffer )
	PointerToNewSection = LPVOID((DWORD)PointerToNewSection + sizeof(_IMAGE_EXPORT_DIRECTORY));

	return PointerToNewSection;
}


VOID ChangeDll_IMAGE_BASE_RELOCATION(IN LPVOID pNewFileBuffer, IN LPVOID PointerToNewSection)
{
	/* 此函数用于移动重定位表并修正数据 */
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pNewFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pPIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pPIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pPIMAGE_NT_HEADERS32 + sizeof(pPIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pPIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pPIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pPIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pPIMAGE_OPTIONAL_HEADER32 + pPIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pPIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	
	// 计算出重定位表的 FOA 并加上 pFileBuffer 的值 赋值给重定位表结构体指针
	PIMAGE_BASE_RELOCATION pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa((pPIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, pNewFileBuffer) + (DWORD)pNewFileBuffer);

	// 计算重定位表的总大小 将结果写入 SizeOf_IMAGE_BASE_RELOCATION 中
	UINT SizeOf_IMAGE_BASE_RELOCATION = 0;

	// 将要从 pNEW_PIMAGE_BASE_RELOCATION 开始读取 写入到 PointerToNewSection 的位置
	LPVOID pNEW_PIMAGE_BASE_RELOCATION = (LPVOID)pPIMAGE_BASE_RELOCATION;

	// 判断是否第一个块有值
	if (!(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress))
		// 如果重定位表没有值则 总大小为 0
		SizeOf_IMAGE_BASE_RELOCATION = 0;

	// 重定位表的总大小 = 每个块的大小相加
	do
	{
		// 如果 VirtualAddress 或者 SizeOfBlock 的值为0 则重定位表结束
		 SizeOf_IMAGE_BASE_RELOCATION += pPIMAGE_BASE_RELOCATION -> SizeOfBlock;

		// 跳过一个块的全部地址 注意这里不需要加 IMAGE_SIZEOF_BASE_RELOCATION
		pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pPIMAGE_BASE_RELOCATION + pPIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}
	while(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress);

	// 移动重定位表到 PointerToNewSection
	memcpy(PointerToNewSection, pNEW_PIMAGE_BASE_RELOCATION, SizeOf_IMAGE_BASE_RELOCATION);

	// 更改 指向 _IMAGE_BASE_RELOCATION 的值 (数据目录中导出表的 RVA ) 
	(pPIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress = ChangeFoaToRva((DWORD)PointerToNewSection - (DWORD)pNewFileBuffer, pNewFileBuffer);

	return;
}


int main(int argc, char* argv[])
{
	/*
		本节练习：
		1、在DLL新增一个节，并将导出表信息移动到这个新的节中.
		2、使用工具打开修改后的DLL 看能否正常解析.
		3、在DLL中新增一个节，并将重定位表移动到这个新的节中.
		4、修改DLL的ImageBase,根据重定位表修正，然后存盘.看DLL是否可以使用.(写在了下一个项目里)
	*/


	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD SIZE_OF_FILE_BUFFER = ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 计算将要生成的DLL需要增加多少字节
	DWORD SIZE_OF_NEW_DLL = SizeOfNewDll(pFileBuffer);

	// 计算新增的节对齐后需要多少空间 (文件对齐/内存对齐)
	DWORD SizeOfNewDll_FileAlignment = MyAlignment(pFileBuffer, 0, SIZE_OF_NEW_DLL);
	DWORD SizeOfNewDll_ImageAlignment = MyAlignment(pFileBuffer, 1, SIZE_OF_NEW_DLL);

	// SizeOfNewDll_FileAlignment 为整个需要申请的空间大小 (导出表及重定位表文件对齐后的大小也就是新增节后的大小)
	DWORD SIZE_OF_ALL_FILE_BUFFER = SizeOfNewDll_FileAlignment + SIZE_OF_FILE_BUFFER;

	// 申请空间并复制数据 总共的需要申请的大小是: 原始的文件大小 + 新增的数据文件对齐后的大小 返回值是 NewFileBuffer 的指针
	LPVOID pNewFileBuffer = CopyFileBufferToNewFileBuffer(pFileBuffer, SIZE_OF_ALL_FILE_BUFFER, SIZE_OF_FILE_BUFFER);
	
	// 新增节并校准数据
	DWORD SizeOfNewImageBuffer = AddNewSection(pNewFileBuffer, SizeOfNewDll_FileAlignment, SizeOfNewDll_ImageAlignment, SIZE_OF_FILE_BUFFER);

	// 移动导出表并校准数据
	LPVOID PointerToNewSection = ChangeDLL_IMAGE_EXPORT_DIRECTORY(pNewFileBuffer);

	// 移动重定位表并校准数据
	ChangeDll_IMAGE_BASE_RELOCATION(pNewFileBuffer, PointerToNewSection);

	// 存盘
	WriteMemeryToFile(pNewFileBuffer, SIZE_OF_ALL_FILE_BUFFER, NEW_PATH);


	// ----------------------------------------------------
	// 测试DLL是否可以正常使用
	TestDll();  // NEW_PATH
	// ----------------------------------------------------
	
	getchar();


	return 0;
}
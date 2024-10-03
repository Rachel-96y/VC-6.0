// Win32Api_6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char PATH[80];

BOOL Modification(LPVOID pFileBuffer)
{
	/*
		修改对话框标题
	*/

	return NULL;
}

BOOL ExtractionIcon(LPVOID pFileBuffer)
{
	/*
		提取图标
	*/

	return NULL;
}

BOOL PrintResource(LPVOID pFileBuffer)
{
	/*
		打印资源表信息
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 判断DOS_MZ头是否有效
	if (pIMAGE_DOS_HEADER -> e_magic != IMAGE_DOS_SIGNATURE)
	{
		printf("这不是一个有效的PE结构: 错误的MZ头标志");
		return NULL;
	}

	// 判断PE标识是否有效
	if (*(LPDWORD)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew) != IMAGE_NT_SIGNATURE)
	{
		printf("这不是一个有效的PE结构: 错误的PE标志");
		return NULL;
	}

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 定义资源表结构体头部指针
	PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY = (IMAGE_RESOURCE_DIRECTORY*)(ChangeRvaToFoa((pIMAGE_DATA_DIRECTORY + 2) -> VirtualAddress, pFileBuffer) + (DWORD)pFileBuffer);

	// 找到有多少个不同的资源类型
	DWORD dwNumberOfTypes = pIMAGE_RESOURCE_DIRECTORY -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY -> NumberOfNamedEntries;

	// 如果没有任何类型则直接返回
	if (dwNumberOfTypes == 0x0000FFFF || *(LPWORD)pIMAGE_RESOURCE_DIRECTORY == IMAGE_DOS_SIGNATURE)
	{
		// 如果类型数量是 0xFFFF 或资源表结构体头部指针 地址中的值是 DOS 头标记(0x4d5a) 则代表没有资源
		printf("没有资源!\n");
		return NULL;
	}

	// 定义资源区段指针
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY + 1);
	do
	{ 
		// 遍历有多少种不同的资源类型

		if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameIsString)
		{
			// 如果高位为0 则此4个字节的后31位就是 资源类型的ID
			printf("******************************************************\n");
			printf("资源类型(ID):%d\t", pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset);
			switch(pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset)
			{

			case RT_CURSOR:
				{
					printf("(光标)\n");
					break;
				}
			case RT_BITMAP:
				{
					printf("(位图)\n");
					break;
				}
			case RT_ICON:
				{
					printf("(图标)\n");
					break;
				}
			case RT_MENU:
				{
					printf("(菜单)\n");
					break;
				}
			case RT_DIALOG:
				{
					printf("(对话框)\n");
					break;
				}
			case RT_STRING:
				{
					printf("(字符串组)\n");
					break;
				}
			case RT_FONTDIR:
				{
					printf("(字体目录)\n");
					break;
				}
			case RT_FONT:
				{
					printf("(字体)\n");
					break;
				}
			case RT_ACCELERATOR:
				{
					printf("(加速器表)\n");
					break;
				}
			case RT_RCDATA:
				{
					printf("(自定义资源)\n");
					break;
				}
			case RT_MESSAGETABLE:
				{
					printf("(消息表)\n");
					break;
				}
			case RT_GROUP_CURSOR:
				{
					printf("(图标组)\n");
					break;
				}
			case RT_GROUP_ICON:
				{
					printf("(光标组)\n");
					break;
				}
			case RT_DLGINCLUDE:
				{
					printf("(获取资源)\n");
					break;
				}
			case RT_VERSION:
				{
					printf("(版本信息)\n");
					break;
				}
			case RT_PLUGPLAY:
				{
					printf("(即插即用资源)\n");
					break;
				}
			case RT_VXD:
				{
					printf("(VXD)\n");
					break;
				}
			case RT_ANICURSOR:
				{
					printf("(动态光标)\n");
					break;
				}
			case RT_ANIICON:
				{
					printf("(动态图标)\n");
					break;
				}
			case RT_HTML:
				{
					printf("(HTML文档)\n");
					break;
				}
			default:
				{
					printf("(未知资源类型)\n");
				}
			};
		}
		else
		{
			// 如果高位为1 则指向 _IMAGE_RESOURCE_DIR_STRING_U 结构 资源类型就是自定义的 是一个字符串
			PIMAGE_RESOURCE_DIR_STRING_U pIMAGE_RESOURCE_DIR_STRING_U = (IMAGE_RESOURCE_DIR_STRING_U*)(pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset + (DWORD)pIMAGE_RESOURCE_DIRECTORY);

			// 资源类型的字符串长度
			DWORD IMAGE_RESOURCE_DIR_STRING_U_Length = pIMAGE_RESOURCE_DIR_STRING_U -> Length;

			// 资源类型的起始位置
			PTCHAR StringOfResource = (PTCHAR)(&pIMAGE_RESOURCE_DIR_STRING_U -> NameString);

			printf("资源类型(名称): ");
			do
			{
				printf("%s", StringOfResource);
				StringOfResource += 2;
			}
			while(--IMAGE_RESOURCE_DIR_STRING_U_Length);
			printf("\n");
		}
		
		// 开始遍历下一层(编号)
		if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY -> DataIsDirectory)
		{
			// 如果高位是0
			printf("这里一定不是0\n");
		}
		else
		{
			// 如果高位是1 找到下一层目录节点的起始位置 (编号的起始地址)
			PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY_Digit = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY -> OffsetToDirectory);
			
			// 编号数量
			DWORD dwNumberOfDigit = pIMAGE_RESOURCE_DIRECTORY_Digit -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY_Digit -> NumberOfNamedEntries;

			// 找到第二层的资源编号区段
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY_Digit + 1);
			do
			{
				if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameIsString)
				{
					// 如果高位为0 则此4个字节的后31位就是 资源编号
					printf("资源编号是:%d\n", pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameOffset);
				}	
				else
				{
					// 如果高位为1 则指向 _IMAGE_RESOURCE_DIR_STRING_U 结构 资源编号就是自定义的 是一个字符串
					PIMAGE_RESOURCE_DIR_STRING_U pIMAGE_RESOURCE_DIR_STRING_U = (IMAGE_RESOURCE_DIR_STRING_U*)(pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameOffset + (DWORD)pIMAGE_RESOURCE_DIRECTORY);

					// 资源类型的字符串长度
					DWORD IMAGE_RESOURCE_DIR_STRING_U_Length = pIMAGE_RESOURCE_DIR_STRING_U -> Length;

					// 资源类型的起始位置
					PTCHAR StringOfResource = (PTCHAR)(&pIMAGE_RESOURCE_DIR_STRING_U -> NameString);

					printf("资源编号(名称): ");
					do
					{
						printf("%s", StringOfResource);
						StringOfResource += 2;
					}
					while(--IMAGE_RESOURCE_DIR_STRING_U_Length);
					printf("\n");
				}

				// 找到代码页
				PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY_CodePage = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> OffsetToDirectory);

				// 代码页数量
				DWORD dwNumberOfCodePage = pIMAGE_RESOURCE_DIRECTORY_CodePage -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY_CodePage -> NumberOfNamedEntries;
				// 找到第三层的代码页区段
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY_CodePage + 1);
				do
				{
					// printf("第三层的 DataIsDirectory :%d\n", pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> DataIsDirectory);

					printf("代码页:%d", pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> NameOffset);
					switch (pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> NameOffset)
					{
					case 1033:
						{
							printf("(英文)\n");
							break;
						}
					case 2052:
						{
							printf("(中文)\n");
							break;
						}
					default:
						{
							printf("(其它语言)\n");
						}
					};
					// 找到节点
					PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> OffsetToDirectory);
					
					printf("Rva: %x\n", pIMAGE_DATA_DIRECTORY -> VirtualAddress);
					printf("OffSet: %x\n", ChangeRvaToFoa(pIMAGE_DATA_DIRECTORY -> VirtualAddress, pFileBuffer));
					printf("Size: %x\n\n", pIMAGE_DATA_DIRECTORY -> Size);
					
					pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage++;
				}
				while(--dwNumberOfCodePage);

				// 找到下一个资源编号的起始位置
				pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element++;
			}
			while(--dwNumberOfDigit);
			
		}
		
		// 找到下一个资源类型的起始位置
		pIMAGE_RESOURCE_DIRECTORY_ENTRY++;
	}
	while(--dwNumberOfTypes);
}

int main(int argc, char* argv[])
{
	/*
		练习：
			1、编写程序，定位某个资源在PE文件中的位置.
			2、编写程序，提取程序图标资源.(找时间做)
			3、编写程序，修改对话框标题.(找时间做)
	*/
	
	printf("请输入不包含空格的文件路径: \n");
	scanf("%s",PATH);

	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD SIZE_OF_FILE_BUFFER = ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 打印指定程序的资源表
	PrintResource(pFileBuffer);

	// 提取指定程序的图标资源
	// ExtractionIcon(pFileBuffer);

	// 修改指定程序的对话框标题
	// Modification(pFileBuffer);
	
	system("pause");

	return 0;
}
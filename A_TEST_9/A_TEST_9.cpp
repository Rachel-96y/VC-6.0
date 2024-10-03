// A_TEST_7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
	/*
		练习：
		代码实现导入表注入
	*/

	// 静态 DLL 注入 (导入表注入) | EXE_PATH (将要进行注入的目标 EXE ) | NEW_EXE_PATH (生成的文件路径)

	printf("请输入目标 EXE 的绝对路径 (可以将 EXE 拖拽进来)\n");
	CHAR EXE_PATH[100] = {0};
	scanf("%s", EXE_PATH);

	printf("请输入要注入的 DLL 的绝对路径 (可以将 DLL 拖拽进来)\n");
	CHAR INJECT_DLL_PATH[100] = {0};
	scanf("%s", INJECT_DLL_PATH);

	// 拼接一个 .bak 字符串作为新路径
	CHAR BAK_PATH[100] = {0};
	strcpy(BAK_PATH, EXE_PATH);
	LPSTR result = &BAK_PATH[0];

	while(TRUE)
	{
		if (!(*result))
		{
			*(result - 3) = 'b';
			*(result - 2) = 'a';
			*(result - 1) = 'k';
			break;
		}
		result++;
	}

	// 先备份 EXE 文件
	LPVOID pFileBuffer_bak = CreateFileBuffer(EXE_PATH);
	if (!pFileBuffer_bak)
	{
		printf("路径不可用 请勿包含空格及双引号\n");
		system("pause");
		return NULL;
	}

	// 读取 EXE 文件到 FileBuffer_bak 中
	DWORD dwSizeOfFileBuffer_bak = ReadPeFileToFileBuffer(EXE_PATH, pFileBuffer_bak);

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer_bak;

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
		printf("这不是一个有效的PE结构: 错误的MZ头标志\n");
		system("pause");
		return NULL;
	}

	// 判断PE标识是否有效
	if (*(PDWORD)((DWORD)pPIMAGE_DOS_HEADER + pPIMAGE_DOS_HEADER -> e_lfanew) != IMAGE_NT_SIGNATURE)
	{
		printf("这不是一个有效的PE结构: 错误的PE标志\n");
		system("pause");
		return NULL;
	}

	// 存盘 新路径为 BAK_PATH (xxx.bak)
	WriteMemeryToFile(pFileBuffer_bak, dwSizeOfFileBuffer_bak, BAK_PATH);

	// 根据文件大小申请一块内存
	LPVOID pFileBuffer = CreateFileBuffer(EXE_PATH);

	// 将 PE 文件读取进 FileBuffer 中 返回值是文件的实际大小
	DWORD dwReadPeFileToFileBuffer = ReadPeFileToFileBuffer(EXE_PATH, pFileBuffer);

	// 如果编译器在 PE 文件最后添加了内容则更改 FileBuffer 指向 (重新申请一块新的空间释放旧的空间)
	LPVOID pFileBufferEx = CleanPeData(pFileBuffer, dwReadPeFileToFileBuffer);

	// 获取 PE 文件在 FileBuffer 中的大小
	DWORD dwSizeOfFileBufferEx = SizeOfFileBuffer(pFileBufferEx);

	// 计算需要注入的 DLL 中的函数名长度 (这里只需要一个DLL 中的一个函数即可)
	DWORD dwFunctionNameLength = SizeOfDllFunctionLength(INJECT_DLL_PATH);

	// 得到 DLL 里的第一个函数的名字 将其存储在 strDllFunctionNameAddr 中
	LPSTR strDllFunctionNameAddr = DllFunctionName(INJECT_DLL_PATH);

	// 计算需要移动的表及需要新增的表总共占多少字节
	DWORD dwSizeOfCodeToInjectDll = SizeOfCodeToInjectDll(pFileBufferEx, INJECT_DLL_PATH, dwFunctionNameLength);

	// 获取模块名称 
	LPSTR FileNameAddr = GetFileName(INJECT_DLL_PATH);
	
	// 根据需要写入的数据大小 计算对齐后的大小
	DWORD dwAlignment = MyAlignment(pFileBufferEx, 0, dwSizeOfCodeToInjectDll);

	// 扩大节 (IsSuccess 为 NULL 则此函数可以正常执行)
	LPVOID pNewFileBuffer = ExpandSection(0xFFFFFFFF, pFileBufferEx, dwAlignment);

	// 导入表注入
	ImportDescriptorInjectDll(pNewFileBuffer, (LPVOID)0xFFFFFFFF, strDllFunctionNameAddr, FileNameAddr, dwSizeOfFileBufferEx);

	// 计算 NewFileBuffer 文件目前的大小
	DWORD dwSizeOfFileBuffer = SizeOfFileBuffer(pNewFileBuffer);

	// 存盘(路径: NEW_EXE_PATH)
	WriteMemeryToFile(pNewFileBuffer, dwSizeOfFileBuffer, EXE_PATH);

	// 完成!
	printf("DLL 注入成功! \n请将 DLL 和 EXE 放在同一目录下或将 DLL 放在系统环境变量中!\n注意: 同目录下已经生成了一个 .bak 文件 (此EXE的备份文件)\n一个 EXE 只能注入一个 DLL\n");

	system("pause");

	return 0;
}
// project_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

BOOL AddCodeToFreeArea()
{
	/* 向代码节添加代码 && 向其他节空白区添加代码 */

	// 定义 ShellCode 数组
	UCHAR CodeArray[CODE_LENGTH] = 
	{	
		0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00,  
		0xE8, 0x00, 0x00, 0x00, 0x00, 
		0xE9, 0x00, 0x00, 0x00, 0x00,
	};  

	// 读取一个文件 根据大小申请空间
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 读取硬盘上的文件到指定内存空间 返回读取的大小
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 根据 FileBuffer 中的 SizeOfImage 成员大小申请一块 ImageBuffer
	LPVOID pImageBuffer = CreateImageBuffer(pFileBuffer);


	// 将 PE 文件 从 FileBuffer 拉伸并放入  ImageBuffer
	DWORD SizeOfCopyFileBufferToImageBuffer = CopyFileBufferToImageBuffer(pFileBuffer, pImageBuffer);

	// 向代码节静态注入代码  
	AddCodeToImageBuffer(&CodeArray[0], 1, CODE_LENGTH, pImageBuffer);

	// 根据 ImageBuffer 中的值 申请一块 NewBuffer 空间
	LPVOID pNewBuffer = CreateNewBuffer(pImageBuffer);

	// 根据 pImageBuffer 和 pNewBuffer 将 ImageBuffer 中的数据拷贝到 NewBuffer 中
	DWORD SizeOfCopyImageBufferToNewBuffer = CopyImageBufferToNewBuffer(pImageBuffer, pNewBuffer);
	
	// 存盘
	WriteMemeryToFile(pNewBuffer, SizeOfCopyImageBufferToNewBuffer, NEW_PATH);

	printf("存盘成功!\n");
	getchar();

	return TRUE;
}

BOOL AddSection()
{
	/* 新增一个节，并添加代码 */

	// 定义 ShellCode 数组
	UCHAR CodeArray[CODE_LENGTH] = 
	{	
		0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00,  
		0xE8, 0x00, 0x00, 0x00, 0x00, 
		0xE9, 0x00, 0x00, 0x00, 0x00,
	}; 

	// 根据文件大小申请空间
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将文件读进指定内存
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 根据 FileBuffer 创建 ImageBuffer
	LPVOID pImageBuffer = CreateImageBuffer(pFileBuffer);

	// 从 FileBuffer 拷贝到 ImageBuffer
	CopyFileBufferToImageBuffer(pFileBuffer, pImageBuffer);

	// 根据需要添加的代码大小 申请一块 NewImageBuffer 空间
	// (需要先判断是否有空间添加1个节表)
	// (总共申请的内存空间为 新增一个节数据后内存对齐的大小)
	LPVOID pNewImageBuffer = CreateNewImageBuffer(pImageBuffer, CODE_LENGTH);

	//将数据从 ImageBuffer 拷贝到 NewImageBuffer 再新增一个节表和一个节 校准全部数据 
	CopyImageBufferToNewImageBuffer(pImageBuffer, pNewImageBuffer, CODE_LENGTH, 0);

	// 向新增的节区静态注入代码
	DWORD result = AddCodeToImageBuffer(&CodeArray[0], 4, CODE_LENGTH, pNewImageBuffer);
	if (!result)
	{
		return NULL;
	}

	// 根据 NewImageBuffer 计算得出 NewBuffer的大小 申请对应空间

	LPVOID pNewBuffer = CreateNewBuffer(pNewImageBuffer);

	// 从 NewImageBuffer 还原到 NewBuffer (使用 CopyImageBufferToNewBuffer 函数可以达到要求)
	DWORD SizeOfCopyByte = CopyImageBufferToNewBuffer(pNewImageBuffer, pNewBuffer);

	// 存盘
	WriteMemeryToFile(pNewBuffer, SizeOfCopyByte, NEW_PATH);

	printf("存盘成功!\n");
	getchar();

	return TRUE;
}

BOOL ExpandSection()
{
	/* 扩大节 扩大最后一个节，并添加代码 */

	// 定义 ShellCode 数组
	UCHAR CodeArray[CODE_LENGTH] = 
	{	
		0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00,  
		0xE8, 0x00, 0x00, 0x00, 0x00, 
		0xE9, 0x00, 0x00, 0x00, 0x00,
	}; 

	// 根据文件大小 申请一块内存空间 FileBuffer
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将文件读进 FileBuffer
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 根据 FileBuffer 申请一块内存空间 ImageBuffer
	LPVOID pImageBuffer = CreateImageBuffer(pFileBuffer);

	// 从 FileBuffer 将数据读进 ImageBuffer
	CopyFileBufferToImageBuffer(pFileBuffer, pImageBuffer);

	// 扩大节 获取代码所对应的对齐后的大小
	DWORD AlignNumber = MyAlignment(pImageBuffer, 0, CODE_LENGTH);

	// 为扩大节申请足够的空间
	LPVOID pNewImageBuffer = CreateNewImageBufferEx(pImageBuffer, AlignNumber);

	// 从 ImageBuffer 将数据复制到 NewImageBuffer
	CopyImageBufferToNewImageBufferEx(pImageBuffer, pNewImageBuffer, AlignNumber, 1, CODE_LENGTH);

	// 静态注入代码
	BOOL result = AddCodeToImageBuffer(&CodeArray[0], 3, CODE_LENGTH, pNewImageBuffer);
	if (!result)
	{
		// 无法注入代码
		return NULL;
	}

	// 根据 NewImageBuffer 申请一块 NewBuffer 的空间 用于存储压缩后的数据
	LPVOID pNewBuffer = CreateNewBuffer(pNewImageBuffer);

	// 将 NewImageBuffer 压缩成在硬盘上的格式
	DWORD SizeOfCopyByte = CopyImageBufferToNewBuffer(pNewImageBuffer, pNewBuffer);

	// 存盘
	WriteMemeryToFile(pNewBuffer, SizeOfCopyByte, NEW_PATH);

	printf("存盘成功!\n");
	getchar();

	return TRUE;
}

BOOL MergeSection()
{
	/* 将所有节合并，并添加代码 */

	// 定义 ShellCode 数组
	UCHAR CodeArray[CODE_LENGTH] = 
	{	
		0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00,  
		0xE8, 0x00, 0x00, 0x00, 0x00, 
		0xE9, 0x00, 0x00, 0x00, 0x00,
	}; 
	
	// 根据文件大小 申请一块内存空间 FileBuffer
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将文件读进 FileBuffer
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

	// 根据 FileBuffer 申请一块内存空间 ImageBuffer
	LPVOID pImageBuffer = CreateImageBuffer(pFileBuffer);

	// 从 FileBuffer 将数据读进 ImageBuffer
	CopyFileBufferToImageBuffer(pFileBuffer, pImageBuffer);

	// 修改 imageBuffer 内的值 合并节
	ChangeImageBufferData(pImageBuffer, 1, CODE_LENGTH);

	// 静态注入代码
	BOOL result = AddCodeToImageBuffer(&CodeArray[0], 1, CODE_LENGTH, pImageBuffer);
	if (!result)
	{
		// 无法注入代码
		return NULL;
	}

	// 根据 NewImageBuffer 申请一块 NewBuffer 的空间 用于存储压缩后的数据
	LPVOID pNewBuffer = CreateNewBuffer(pImageBuffer);

	// 将 NewImageBuffer 压缩成在硬盘上的格式
	DWORD SizeOfCopyByte = CopyImageBufferToNewBuffer(pImageBuffer, pNewBuffer);

	// 存盘
	WriteMemeryToFile(pNewBuffer, SizeOfCopyByte, NEW_PATH);

	printf("存盘成功!\n");
	getchar();

	return TRUE;
}

BOOL PrintOfDirectoryEntry()
{
	/* 输出全部目录项 */

	// 导出表
	// 导入表
	// 资源表
	// 异常表
	// 安全证书表
	// 重定位表
	// 调试信息表
	// 版权所有表
	// 全局指针表
	// TLS表
	// 加载配置表
	// 绑定导入表
	// IAT表
	// 延迟导入表
	// COM表
	// 保留表
	
	// 根据文件大小 申请一块内存空间 FileBuffer
	LPVOID pFileBuffer = CreateFileBuffer(PATH);

	// 将文件读进 FileBuffer
	ReadPeFileToFileBuffer(PATH, pFileBuffer);

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
	
	// 找到数据目录的位置并定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pPIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)\
		((DWORD)pPIMAGE_SECTION_HEADER - IMAGE_NUMBEROF_DIRECTORY_ENTRIES * sizeof(IMAGE_DATA_DIRECTORY));
	
	char* arr[16] = 
	{
		"导出表", "导入表", "资源表", "异常信息表", 
		"安全证书表", "重定位表", "调试信息表", "版权所有表", 
		"全局指针表", "TLS表", "加载配置表", "绑定导入表",
		"IAT表", "延迟导入表", "COM信息表", "保留且未使用的表",
	};

	for (int i = 0; i < 16; i++)
	{
		printf("%s:\nRVA:%x  Size:%x\n\n", arr[i], \
			(pPIMAGE_DATA_DIRECTORY + i) -> VirtualAddress, (pPIMAGE_DATA_DIRECTORY + i) -> Size);
	}

	getchar();

	return TRUE;
}



int main(int argc, char* argv[])
{
	/*
		练习：

		1、扩大最后一个节，保证程序正常运行.

		2、编程实现：扩大最后一个节，并添加代码.

		3、将所有节合并，保证程序正常运行.

		4、定义一个函数，能够返回对齐后的大小 Alignment(int x,int y)

		5、编程输出全部目录项.

	*/
	
	// AddCodeToFreeArea();		// 向代码节添加代码 && 向其他节空白区添加代码

	// AddSection();		// 新增节并添加代码

	// ExpandSection();			// 扩大节并添加代码

	// MergeSection();		// 合并节并添加代码
	
	// PrintOfDirectoryEntry();  // 输出全部目录项
	

	return 0;
}
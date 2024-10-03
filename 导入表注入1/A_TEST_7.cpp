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
	
	// 计算节的空白区大小 能否足够写入数据
	LPVOID pCodeBegin = IsEnoughSpace(pFileBufferEx, dwSizeOfCodeToInjectDll);

	// 获取模块名称 
	LPSTR FileNameAddr = GetFileName(INJECT_DLL_PATH);

	// 导入表注入 (pCodeBegin 为 NULL 则将注入失败 如果失败那么返回值为 0xFFFFFFFF)
	DWORD IsSuccess = ImportDescriptorInjectDll(pFileBufferEx, pCodeBegin, strDllFunctionNameAddr, FileNameAddr, dwSizeOfFileBufferEx);

	// 根据需要写入的数据大小 计算对齐后的大小
	DWORD dwAlignment = MyAlignment(pFileBufferEx, 0, dwSizeOfCodeToInjectDll);

	// 扩大节 (IsSuccess 为 NULL 则此函数可以正常执行)
	LPVOID pNewFileBuffer = ExpandSection(IsSuccess, pFileBufferEx, dwAlignment);

	// 再次执行导入表注入
	ImportDescriptorInjectDll(pNewFileBuffer, (LPVOID)IsSuccess, strDllFunctionNameAddr, FileNameAddr, dwSizeOfFileBufferEx);

	// 计算 NewFileBuffer 文件目前的大小
	DWORD dwSizeOfFileBuffer = SizeOfFileBuffer(pNewFileBuffer);

	// 存盘(路径: NEW_EXE_PATH)
	WriteMemeryToFile(pNewFileBuffer, dwSizeOfFileBuffer, NEW_EXE_PATH);


	getchar();

	return 0;
}
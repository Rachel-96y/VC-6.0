// stdafx.cpp : source file that includes just the standard includes
//	Other_CPP_1.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

// *******************************************************************************
//打开文件 并申请内存
//参数OUT LPVOID* pFileBuffer 传出LPVOID类型的 pFileBuffer指针
//返回值为  缓存的长度
DWORD CreateFileBuffer(IN LPSTR lpszFile, OUT LPVOID* pFileBuffer) 
{
	FILE* pfile = 0;
	DWORD filesize = 0;
	LPVOID FileBuffer = 0;
	pfile = fopen(lpszFile, "rb");
	if (!pfile) {
		printf("文件打开失败!!");
		return 0;
	}

	//指针指向pfile文件尾部;
	fseek(pfile, 0, SEEK_END);
	//取出长度

	filesize = ftell(pfile);

	//返回指针 指向pfile文件头部
	fseek(pfile, 0, SEEK_SET);
	
	//申请内存 并判断是否成功
	FileBuffer = malloc(filesize);
	if (!FileBuffer) {
		printf("内存申请失败!!");
		fclose(pfile);
		pfile = NULL;
		return 0;
	}

	memset(FileBuffer, 0, filesize);

	//复制文件并 判断
	DWORD x = fread(FileBuffer, sizeof(CHAR), filesize, pfile);
	if (!x) {
		printf("文件复制至pFileBuffer失败!!");
		fclose(pfile);
		free(FileBuffer);
		return NULL;
	}

	//判断是否符合MZ标记!!
	if (*((WORD*)FileBuffer) != IMAGE_DOS_SIGNATURE) {
		printf("此文件不是MZ标记文件 或文件已损坏!!");
		fclose(pfile);
		free(FileBuffer);
		return 0;
	}

	*pFileBuffer = FileBuffer;
	fclose(pfile);
	return filesize;
 }


// *******************************************************************************
//把FileBuffer缓存内容拉伸至ImageBuffer
//参数1 传入 FileBuffer 首地址指针
//参数2 传出 ImageBuffer 首地址指针
//返回值 ImageBuffer的长度
DWORD CopyFileBufferToImageBuffer(IN LPVOID pFileBuffer, OUT LPVOID* pImageBuffer) {

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

	//计算 ImageBuffer 需要申请内存的长度
	DWORD ffersize = pPIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;

	//申请ImageBuffer内存 并判断
	LPVOID ImageBuffer = malloc(ffersize);
	if (!ImageBuffer) {
		printf("ImageBuffer内存申请失败!!\n");
		free(pFileBuffer);
		return 0;
	}

	//初始化缓存  拷贝所有头
	memset(ImageBuffer, 0, ffersize);
	memcpy(ImageBuffer, pFileBuffer, pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);

	//赋值节表指针 循环拷贝节
	for (int i = 0; i < pPIMAGE_FILE_HEADER -> NumberOfSections; i++) 
	{
		memcpy((LPVOID)((DWORD)ImageBuffer + pPIMAGE_SECTION_HEADER -> VirtualAddress), \
			(LPVOID)((DWORD)pFileBuffer + pPIMAGE_SECTION_HEADER -> PointerToRawData), \
			pPIMAGE_SECTION_HEADER -> SizeOfRawData);
		pPIMAGE_SECTION_HEADER++;
	}

	// 给 pImageBuffer 指针赋值
	*pImageBuffer = ImageBuffer;

	return ffersize;
 }


// *******************************************************************************
//ImageBuffer缓存内容按文件格式拷贝至NewBuffer
//参数1 传入ImageBuffer首地址指针
//参数2 传出NewBuffer首地址指针
//返回值 NewBuffer的长度
DWORD ImageBufferToNewBuffer(IN LPVOID ImageBuffer, OUT LPVOID* pNewBuffer) {

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pPIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)ImageBuffer;

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

	// 定位到最后一个节
	pPIMAGE_SECTION_HEADER += (pPIMAGE_FILE_HEADER -> NumberOfSections - 1);

	//计算 NewBuffer 申请内存的长度
	DWORD SizeOfNewBuffer = pPIMAGE_SECTION_HEADER -> PointerToRawData + pPIMAGE_SECTION_HEADER -> SizeOfRawData;

	//申请内存
	LPVOID NewBuffer = malloc(SizeOfNewBuffer);
	if (!NewBuffer) {
		printf("NewBuffer 内存申请失败!!");
		free(ImageBuffer);
		return 0;
	}

	// 节表结构体指针复位
	pPIMAGE_SECTION_HEADER -= (pPIMAGE_FILE_HEADER -> NumberOfSections - 1);

	// 初始化
	memset(NewBuffer, 0, SizeOfNewBuffer);

	// 拷贝头
	memcpy(NewBuffer, ImageBuffer, pPIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);

	// 拷贝节
	for (int i = 0; i < pPIMAGE_FILE_HEADER -> NumberOfSections; i++) {
		memcpy((LPVOID)((DWORD)NewBuffer + pPIMAGE_SECTION_HEADER -> PointerToRawData), \
			(LPVOID)((DWORD)ImageBuffer + pPIMAGE_SECTION_HEADER -> VirtualAddress), \
			pPIMAGE_SECTION_HEADER -> SizeOfRawData);
		pPIMAGE_SECTION_HEADER++;
	}

	//free(ImageBuffer);
	*pNewBuffer = NewBuffer;
	return SizeOfNewBuffer;
}


// *******************************************************************************
//存盘函数
//参数1 传入NewBuffer的指针
//参数2 传入NewBuffer 长度
//返回值 成功返回1 失败返回0;

DWORD WriteMemeryToFile(IN LPVOID NewBuffer, IN DWORD file_size) 
{
	FILE* pf = fopen("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\NEW_notepad.exe", "wb");
	if (!pf) {
		printf("存盘的文件创建失败");
		return 0;
	}
	DWORD x = fwrite(NewBuffer, sizeof(CHAR), file_size, pf);
	if (!x) {
		printf("文件存盘失败");
		fclose(pf);
		return 0;
	}
	printf("文件存盘成功");
	fclose(pf);
	return 1;
}



// *******************************************************************************


//RVA  FOA  VA转换
void convertFoaToRva(IN LPSTR lpszFile) {
	//打开文件申请和拷贝ImageBuffer缓存
	PVOID FileBuffer = 0;
	PVOID ImageBuffer = 0;
	CreateFileBuffer(IN lpszFile,OUT &FileBuffer);
	CopyFileBufferToImageBuffer(IN FileBuffer, OUT &ImageBuffer);
	//判断是否符合MZ标记!!
	if (*((WORD*)ImageBuffer) != IMAGE_DOS_SIGNATURE) {
		printf("此文件不是MZ标记文件 或文件已损坏!!");
		free(ImageBuffer);
	}
	PIMAGE_DOS_HEADER pdosHeader = (PIMAGE_DOS_HEADER)ImageBuffer;
	PIMAGE_NT_HEADERS pntheader = (PIMAGE_NT_HEADERS)((BYTE*)pdosHeader + pdosHeader->e_lfanew);
	if (*((DWORD*)pntheader) != IMAGE_NT_SIGNATURE) {
		printf("此文件不是PE格式文件 或文件已损坏!!");
		free(ImageBuffer);
	}
	PIMAGE_FILE_HEADER ppeheader = (PIMAGE_FILE_HEADER) & (pntheader->FileHeader);
	PIMAGE_OPTIONAL_HEADER poptionHeader = (PIMAGE_OPTIONAL_HEADER) & (pntheader->OptionalHeader);
	PIMAGE_SECTION_HEADER psectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)poptionHeader + ppeheader->SizeOfOptionalHeader);
	
	
	//输入ImageBuffer的某一个地址
	printf("pImageBuffer:%x\n", ImageBuffer);
	printf("请输入va地址:");
	DWORD Va = 0; 
	scanf("%x", &Va);
	DWORD Rva = Va - (DWORD)ImageBuffer;
	//printf("%x -%x=%x", Va, (DWORD)ImageBuffer,Rva);
	DWORD Foa = 0;
	DWORD base = 0;
	//判断地址是在所有头里面还是在节里面的哪个位置
   //打印在文件的的地址
   //打印在运行状态的地址
	if (Va > (DWORD)ImageBuffer && Va < poptionHeader->SizeOfHeaders) {
		Foa = Rva;
		printf("Foa 的地址为:%x\n", Foa);
		printf("运行状态的地址为:%x\n", Rva+poptionHeader->ImageBase);
	}if (Va>= (DWORD)ImageBuffer +poptionHeader->SizeOfHeaders && Va< (DWORD)ImageBuffer+poptionHeader->SizeOfImage) {
		for (size_t i = 0; i < ppeheader->NumberOfSections; i++,psectionHeader++) {
			if (Rva>=psectionHeader->VirtualAddress && Rva< psectionHeader->VirtualAddress+psectionHeader->SizeOfRawData)  {
				base = Rva + poptionHeader->ImageBase;
				Rva -= psectionHeader->VirtualAddress;
				Foa = Rva + psectionHeader->PointerToRawData;
				printf("Foa 的地址为:%x\n", Foa);
				printf("运行状态的地址为:%x\n", base);
				break;
			}

		}
		free(ImageBuffer);
	}

}


// *******************************************************************************

void ChangeImageBufferCode1(IN LPSTR lpszFile, IN BYTE* ShellCode,IN DWORD Codesize,IN DWORD Maddress) 
{
	/*
		lpszFile		目标 EXE 程序路径
		ShellCode		要注入的代码的指针
		Codesize		要注入的代码的长度
		Maddress		MessageBoxA 函数地址
	*/

	PVOID pFileBuffer = 0;
	PVOID pImageBuffer = 0;
	CreateFileBuffer(IN lpszFile, OUT &pFileBuffer);
	CopyFileBufferToImageBuffer(IN pFileBuffer, OUT &pImageBuffer);
	if (*((WORD*)pImageBuffer) != IMAGE_DOS_SIGNATURE) {
		printf("此文件不是MZ标记文件 或文件已损坏!!");
		fclose((FILE*)pImageBuffer);
	}
	PIMAGE_DOS_HEADER pdosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pntheader = (PIMAGE_NT_HEADERS)((BYTE*)pdosHeader + pdosHeader -> e_lfanew);
	if (*((DWORD*)pntheader) != IMAGE_NT_SIGNATURE) {
		printf("此文件不是PE格式文件 或文件已损坏!!");
		fclose((FILE*)pImageBuffer);
	}
	PIMAGE_FILE_HEADER ppeheader = (PIMAGE_FILE_HEADER) & (pntheader -> FileHeader);
	PIMAGE_OPTIONAL_HEADER poptionHeader = (PIMAGE_OPTIONAL_HEADER) & (pntheader -> OptionalHeader);
	PIMAGE_SECTION_HEADER psectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)poptionHeader + ppeheader -> SizeOfOptionalHeader);
	//printf("%x", psectionHeader->Misc);
	//定位pFileBuffer节区位置
	PIMAGE_SECTION_HEADER pfileBufferpsp = (PIMAGE_SECTION_HEADER)((BYTE*)psectionHeader - (BYTE*)pImageBuffer + (BYTE*)pFileBuffer);
	//判断pFileBuffer 文件节区内空白空间是否足够添加代码
	if (pfileBufferpsp -> Misc.VirtualSize > pfileBufferpsp -> SizeOfRawData) {
		printf("节区无可用空闲内存不可添加!!!!");
	}if (pfileBufferpsp -> SizeOfRawData - pfileBufferpsp -> Misc.VirtualSize < Codesize) {
		printf("节区内存空闲区长度不够添加该代码!!!!");
	}
	
	PBYTE Codbigin = (PBYTE)((DWORD)pImageBuffer + psectionHeader -> VirtualAddress + psectionHeader -> Misc.VirtualSize);
	// printf("%x+%x=%x\n", pImageBuffer, psectionHeader -> VirtualAddress + psectionHeader -> Misc.VirtualSize, Codbigin);
	memcpy(Codbigin, ShellCode, Codesize);

	/*
	DWORD calladdress = (Maddress - (((DWORD)(Codbigin+0xd) - (DWORD)pImageBuffer) + poptionHeader -> ImageBase));
	*(PDWORD)(Codbigin + 0x9) = calladdress;
	// 我写的
	// DWORD jmpsddress = (poptionHeader -> AddressOfEntryPoint - (((DWORD)(Codbigin + Codesize - 1) - (DWORD)pImageBuffer) + poptionHeader -> ImageBase));
	// 我抄的
	DWORD jmpsddress = ((poptionHeader -> ImageBase + poptionHeader -> AddressOfEntryPoint) - (poptionHeader -> ImageBase + ((DWORD)Codbigin + Codesize - (DWORD)pImageBuffer)));
	*(PDWORD)(Codbigin + 0xe) = jmpsddress;
	*/
	
	// 找到 Codbigin 在内存中的偏移地址 + ImageBase
	DWORD VirtualCodbigin = ((DWORD)Codbigin - (DWORD)pImageBuffer) + poptionHeader -> ImageBase;
	printf("%d\n", VirtualCodbigin);
	// 先校准 E8 后面的地址 
	DWORD calladdress = Maddress - (VirtualCodbigin + 0x8 + 0x5);
	printf("%x\n", Maddress);
	// 校准 E9 后面的地址 
	DWORD jmpsddress = poptionHeader -> AddressOfEntryPoint + poptionHeader -> ImageBase - ((DWORD)VirtualCodbigin + 0x8 + 0x5 + 0x5);

	
	// 将 校准后的 E8/E9后面的值 写入 ImageBuffer 对应位置
	*(LPDWORD)(Codbigin + 0x9) = calladdress;
	*(LPDWORD)(Codbigin + 0xE) = jmpsddress;

	poptionHeader -> AddressOfEntryPoint = ((DWORD)Codbigin - (DWORD)pImageBuffer);
	//printf("%x-%x=%x\n", (DWORD) Codbigin, (DWORD)pImageBuffer, poptionHeader -> AddressOfEntryPoint);
	//把拉伸后的文件 在次收缩为文件类型
	PVOID pNewBuffer = 0;
	DWORD file_size = ImageBufferToNewBuffer(pImageBuffer, &pNewBuffer);

	//文件存盘
	WriteMemeryToFile(pNewBuffer, file_size);
	free(pFileBuffer);
	free(pImageBuffer);
	free(pNewBuffer);
}
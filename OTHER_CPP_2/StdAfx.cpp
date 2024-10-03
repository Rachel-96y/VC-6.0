// stdafx.cpp : source file that includes just the standard includes
//	OTHER_CPP_2.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
#include <iostream>
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

/*
// ************ 注释 ************
// 2进制打印调试使用		
void 调试(IN PVOID Buffer, IN WORD file_size) {
	BYTE* y = (BYTE*)Buffer;
	for (size_t i = 0; i < file_size; i++) {
		if (i % 16 == 0) {
			printf("\n");
		}
		printf("%x ", *y++);
	}
}
*/

//打开文件 并申请内存
//参数OUT PVOID* pFileBuffer 传出PVOID类型的 pFileBuffer指针
//返回值为  缓存的长度
DWORD CreateFileBuffer(IN LPSTR lpszFile,OUT PVOID* pFileBuffer) {
	FILE* pfile = 0;
	// ************ 更改 ************
	// WORD filesize = 0;		
	DWORD filesize = 0;
	PVOID FileBuffer = 0;
	pfile = fopen(lpszFile, "rb");
	if (!pfile) {
		printf("文件打开失败!!");
		pfile = 0;
		return 0;
	}

	//指针指向pfile文件尾部;
	fseek(pfile, 0, SEEK_END);
	//取出长度
	filesize = ftell(pfile);
	//返回指针 指向pfile文件头部
	fseek(pfile, 0,SEEK_SET);
	
	//申请内存 并判断是否成功
	FileBuffer =malloc(filesize);
	if (!FileBuffer) {
		printf("内存申请失败!!");
		fclose(pfile);
		return 0;
	}
	memset(FileBuffer, 0, filesize);
	//复制文件并 判断
	DWORD	x = fread(FileBuffer, filesize, 1, pfile);
	if (!x) {
		printf("文件复制至pFileBuffer失败!!");
		fclose(pfile);
		free(FileBuffer);
		return 0;
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


//打印所有头的信息
//参数 传入缓存FileBuffer指针
void ToLoaderPE(IN PVOID pFileBuffer) {
	FILE* pbfer = (FILE*)pFileBuffer;
	PIMAGE_DOS_HEADER  pdosheader = NULL;
	PIMAGE_NT_HEADERS pntheader = NULL;
	PIMAGE_FILE_HEADER ppeheader = NULL;
	PIMAGE_OPTIONAL_HEADER poptionheader = NULL;
	PIMAGE_SECTION_HEADER psectionheader = NULL;
	if (!pbfer) {
		printf("指针读取失败!!");
		free(pbfer);
	}
	if (*((WORD*)pbfer) != IMAGE_DOS_SIGNATURE) {
		printf("不是有效标的MZ标记!!");
		free(pbfer);
	}
	pdosheader = (PIMAGE_DOS_HEADER)pbfer;
	printf("------------------DOS-----------------------\n");
	printf("MZ标记  e_magic:   %x\n", pdosheader->e_magic);
	printf("PE头地址  e_lfanew:   %x\n", pdosheader->e_lfanew);
	printf("------------------NT-----------------------\n");
	if (*((DWORD*)((char*)pbfer + pdosheader->e_lfanew)) != IMAGE_NT_SIGNATURE) {
		printf("不是有效标的PE标记!!");
		free(pbfer);
	}
	pntheader = (PIMAGE_NT_HEADERS)((char*)pdosheader + pdosheader->e_lfanew);
	printf("Signature:%x\n", pntheader->Signature);
	printf("------------------PE-----------------------\n");
	ppeheader = (PIMAGE_FILE_HEADER) & (pntheader->FileHeader);
	printf("CPU型号  Machine:   %x\n", ppeheader->Machine);
	printf("节的总数 NumberOfSections:   %x\n", ppeheader->NumberOfSections);
	printf("时间戳  TimeDateStamp:   %x\n", ppeheader->TimeDateStamp);
	printf("可选头大小  SizeOfOptionalHeader:   %x\n", ppeheader->SizeOfOptionalHeader);
	printf("每个位特征码  Characteristics:   %x\n", ppeheader->Characteristics);
	printf("------------------OPPE-----------------------\n");
	poptionheader = (PIMAGE_OPTIONAL_HEADER) & (pntheader->OptionalHeader);
	printf("类型说明  Magic:   %x\n", poptionheader->Magic);
	printf("所有代码节的和  SizeOfCode:   %x\n", poptionheader->SizeOfCode);
	printf("已初始化数据  SizeOfInitializedData:   %x\n", poptionheader->SizeOfInitializedData);
	printf("未始化数据  SizeOfUninitializedData:   %x\n", poptionheader->SizeOfUninitializedData);
	printf("程序入口  AddressOfEntryPoint:   %x\n", poptionheader->AddressOfEntryPoint);
	printf("代码段基址  BaseOfCode:   %x\n", poptionheader->BaseOfCode);
	printf("数据段基址  BaseOfCode:   %x\n", poptionheader->BaseOfData);
	printf("内存镜像基址  ImageBase:   %x\n", poptionheader->ImageBase);
	printf("内存对齐  SectionAlignment:   %x\n", poptionheader->SectionAlignment);
	printf("文件对齐  FileAlignment:   %x\n", poptionheader->FileAlignment);
	printf("内存PE映射尺寸  SizeOfImage:   %x\n", poptionheader->SizeOfImage);
	printf("头+节表文件对齐  SizeOfHeaders:   %x\n", poptionheader->SizeOfHeaders);
	printf("校验和判断文件修改  CheckSum:   %x\n", poptionheader->CheckSum);
	printf("初始化保留栈大小  SizeOfStackReserve:   %x\n", poptionheader->SizeOfStackReserve);
	printf("初始化实际大小  SizeOfStackCommit:   %x\n", poptionheader->SizeOfStackCommit);
	printf("初始化保留堆大小  SizeOfHeapReserve:   %x\n", poptionheader->SizeOfHeapReserve);
	printf("初始化实际堆大小  SizeOfHeapCommit:   %x\n", poptionheader->SizeOfHeapCommit);
	printf("------------------节表-----------------------\n\n\n");
	psectionheader = (PIMAGE_SECTION_HEADER)((char*)poptionheader + ppeheader->SizeOfOptionalHeader);
	int x = 0;
	int z = 0;
	char y[7] = { 0 };
	//printf("%x", ppeheader->NumberOfSections);
	for (size_t i = 0; i < ppeheader->NumberOfSections; i++, psectionheader++) {
		printf("------------------节表%d-----------------------\n", ++z);
		printf("节的名称  Name: ");
		while (x < 6) {
			y[x] = psectionheader->Name[x];
			printf("%c", y[x]);
			x++;
		}
		x = 0;
		printf("\n");
		printf("文件未对齐尺寸  Misc:   %x\n", psectionheader->Misc);
		printf("内存节偏移地址  VirtualAddress:   %x\n", psectionheader->VirtualAddress);
		printf("文件对齐后尺寸  SizeOfRawData:   %x\n", psectionheader->SizeOfRawData);
		printf("节在文件中偏移  PointerToRawData:   %x\n", psectionheader->PointerToRawData);
		printf("节的属性  Characteristics:   %x\n", psectionheader->Characteristics);
	}

}

//把FileBuffer缓存内容拉伸至ImageBuffer
//参数1 传入FileBuffer首地址指针
//参数2 传出ImageBuffer首地址指针
//返回值 ImageBuffer的长度

DWORD CopyFileBufferToImageBuffer(IN PVOID pFileBuffer, OUT PVOID* pImageBuffer) {

	//判断是否符合MZ标记!!
	if (*((WORD*)pFileBuffer) != IMAGE_DOS_SIGNATURE) {
		printf("此文件不是MZ标记文件 或文件已损坏!!");
		free(pFileBuffer);
		return 0;
	}
	PIMAGE_DOS_HEADER pdosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	PIMAGE_NT_HEADERS pntheader = (PIMAGE_NT_HEADERS)((BYTE*)pdosHeader + pdosHeader->e_lfanew);
	if (*((DWORD*)pntheader)!= IMAGE_NT_SIGNATURE) {
		printf("此文件不是PE格式文件 或文件已损坏!!");
		free(pFileBuffer);
		return 0;
	}
	PIMAGE_FILE_HEADER ppeheader = (PIMAGE_FILE_HEADER) & (pntheader -> FileHeader);
	PIMAGE_OPTIONAL_HEADER poptionHeader = (PIMAGE_OPTIONAL_HEADER) & (pntheader -> OptionalHeader);
	PIMAGE_SECTION_HEADER psectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)poptionHeader + ppeheader -> SizeOfOptionalHeader);
	//计算ImageBuffer需要申请内存的长度
	DWORD ffersize = poptionHeader->SizeOfImage;
	//申请ImageBuffer内存 并判断
	PVOID ImageBuffer = malloc(ffersize);
	if (!ImageBuffer) {
		printf("ImageBuffer内存申请失败!!");
		free(pFileBuffer);
		return 0;
	}
	//初始化缓存  拷贝所有头
	memset(ImageBuffer, 0, ffersize);
	memcpy(ImageBuffer, pFileBuffer, poptionHeader -> SizeOfHeaders);
	//赋值节表指针 循环拷贝节表
	PIMAGE_SECTION_HEADER psectionHeaderTemp = psectionHeader;
	for (size_t i = 0; i < ppeheader -> NumberOfSections; i++, psectionHeaderTemp++) {
		memcpy((PVOID)((BYTE*)ImageBuffer + psectionHeaderTemp->VirtualAddress),
			(PVOID)((BYTE*)pFileBuffer + psectionHeaderTemp->PointerToRawData), psectionHeaderTemp->SizeOfRawData);
	}
	*pImageBuffer = ImageBuffer;
	//free(pFileBuffer);
	return ffersize;
 }


//ImageBuffer缓存内容按文件格式拷贝至NewBuffer
//参数1 传入ImageBuffer首地址指针
//参数2 传出NewBuffer首地址指针
//返回值 NewBuffer的长度

DWORD ImageBufferToNewBuffer(IN PVOID ImageBuffer, OUT PVOID* pNewBuffer) {
	//判断是否符合MZ标记!!
	if (*((WORD*)ImageBuffer) != IMAGE_DOS_SIGNATURE) {
		printf("此文件不是MZ标记文件 或文件已损坏!!");
		free(ImageBuffer);
		return 0;
	}
	PIMAGE_DOS_HEADER pdosHeader = (PIMAGE_DOS_HEADER)ImageBuffer;
	PIMAGE_NT_HEADERS pntheader = (PIMAGE_NT_HEADERS)((BYTE*)pdosHeader + pdosHeader->e_lfanew);
	if (*((DWORD*)pntheader) != IMAGE_NT_SIGNATURE) {
		printf("此文件不是PE格式文件 或文件已损坏!!");
		free(ImageBuffer);
		return 0;
	}
	PIMAGE_FILE_HEADER ppeheader = (PIMAGE_FILE_HEADER)&pntheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER poptionHeader = (PIMAGE_OPTIONAL_HEADER)&pntheader->OptionalHeader;
	PIMAGE_SECTION_HEADER psectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)poptionHeader + ppeheader->SizeOfOptionalHeader);
	// 计算NewBuffer申请内存的长度

	PIMAGE_SECTION_HEADER psp = (PIMAGE_SECTION_HEADER)(psectionHeader + (ppeheader->NumberOfSections - 1));
	
	// ************ 更改 ************
	// DWORD NewBuffersize = psp->PointerToRawData + poptionHeader->SizeOfHeaders+psectionHeader->SizeOfRawData;
	DWORD NewBuffersize = psp -> PointerToRawData + psp -> SizeOfRawData;
	// 申请内存
	PVOID NewBuffer = malloc(NewBuffersize);
	if (!NewBuffer) {
		printf("NewBuffer 内存申请失败!!");
		free(ImageBuffer);
		return 0;
	}
	memset(NewBuffer, 0, NewBuffersize);
	memcpy(NewBuffer, ImageBuffer, poptionHeader->SizeOfHeaders);
	for (size_t i = 0; i < ppeheader->NumberOfSections; i++, psectionHeader++) {
		memcpy((PVOID)((BYTE*)NewBuffer + psectionHeader->PointerToRawData),
			(PVOID)((BYTE*)ImageBuffer + psectionHeader->VirtualAddress), psectionHeader->SizeOfRawData);
	}
	//free(ImageBuffer);
	*pNewBuffer = NewBuffer;
	return NewBuffersize;
}

//存盘函数
//参数1 传入NewBuffer的指针
//参数2 传入NewBuffer 长度
//返回值 成功返回1 失败返回0;
DWORD WriteMemeryToFile(IN PVOID NewBuffer, IN DWORD file_size) {
	// ************ 更改 ************
	// FILE* pf = fopen("D:\\BaiduNetdiskDownload\\关键词6.exe", "wb");		
	FILE* pf = fopen("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\NEW_notepad.exe", "wb");
	if (!pf) {
		printf("存盘的文件创建失败");
		return 0;
	}
	DWORD x = fwrite((FILE*)NewBuffer, file_size, 1, pf);
	if (!x) {
		printf("文件存盘失败");
		fclose(pf);
		return 0;
	}
	printf("文件存盘成功");
	fclose(pf);
	return 1;
}

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

//节内空白区添加代码
/*void ChangeImageBufferCode(IN LPSTR lpszFile, IN BYTE* CodeSize) {
	//打开文件申请和拷贝ImageBuffer缓存
	PVOID FileBuffer = 0;
	PVOID pImageBuffer = 0;
	PVOID pNewBuffer = 0;
	CreateFileBuffer(IN lpszFile, OUT & FileBuffer);
	CopyFileBufferToImageBuffer(IN FileBuffer, OUT & pImageBuffer);
	
	//判断是否符合MZ标记!!
	if (*((WORD*)pImageBuffer) != IMAGE_DOS_SIGNATURE) {
		printf("此文件不是MZ标记文件 或文件已损坏!!");
		fclose((FILE*)pImageBuffer);
	}
	PIMAGE_DOS_HEADER pdosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pntheader = (PIMAGE_NT_HEADERS)((BYTE*)pdosHeader + pdosHeader->e_lfanew);
	if (*((DWORD*)pntheader) != IMAGE_NT_SIGNATURE) {
		printf("此文件不是PE格式文件 或文件已损坏!!");
		fclose((FILE*)pImageBuffer);
	}
	PIMAGE_FILE_HEADER ppeheader = (PIMAGE_FILE_HEADER) & (pntheader->FileHeader);
	PIMAGE_OPTIONAL_HEADER poptionHeader = (PIMAGE_OPTIONAL_HEADER) & (pntheader->OptionalHeader);
	PIMAGE_SECTION_HEADER psectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)poptionHeader + ppeheader->SizeOfOptionalHeader);
	DWORD BoxA = 0x74C828B0;

	for (size_t i = 0; i < ppeheader->NumberOfSections; i++) {
		
		if (psectionHeader->Misc.VirtualSize < psectionHeader->SizeOfRawData && 
			psectionHeader->SizeOfRawData- psectionHeader->Misc.VirtualSize>19) {
			memcpy((PVOID)((BYTE*)pImageBuffer + psectionHeader->VirtualAddress + psectionHeader->Misc.VirtualSize), (FILE*)CodeSize, 18);
			DWORD* E8cod = (DWORD*)((BYTE*)pImageBuffer + psectionHeader->VirtualAddress + psectionHeader->Misc.VirtualSize + 9);
			DWORD codc = BoxA -(DWORD) ((BYTE*)E8cod + 4);
			*E8cod = codc;
			DWORD* E9cod = (DWORD*)((BYTE*)E8cod + 5);
			codc=poptionHeader->AddressOfEntryPoint - (DWORD)((BYTE*)E9cod + 4);
			*E9cod = codc;
			poptionHeader->AddressOfEntryPoint = poptionHeader->ImageBase + (DWORD)((BYTE*)pImageBuffer + psectionHeader->VirtualAddress + psectionHeader->Misc.VirtualSize);
			break;
		}
		DWORD file_size =ImageBufferToNewBuffer(pImageBuffer, &pNewBuffer);
		WriteMemeryToFile(IN pNewBuffer, IN  file_size);
		fclose((FILE*)pNewBuffer);
	}


}*/


// ************ 更改 ************
// void ChangeImageBufferCode1(IN LPSTR lpszFile, IN BYTE* ShellCode,IN DWORD Codesize,IN WORD Maddress) {		
void ChangeImageBufferCode1(IN LPSTR lpszFile, IN BYTE* ShellCode,IN DWORD Codesize,IN DWORD Maddress) {
	PVOID pFileBuffer = 0;
	PVOID pImageBuffer = 0;
	CreateFileBuffer(IN lpszFile, OUT &pFileBuffer);
	CopyFileBufferToImageBuffer(IN pFileBuffer, OUT &pImageBuffer);
	if (*((WORD*)pImageBuffer) != IMAGE_DOS_SIGNATURE) {
		printf("此文件不是MZ标记文件 或文件已损坏!!");
		fclose((FILE*)pImageBuffer);
	}
	PIMAGE_DOS_HEADER pdosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
	PIMAGE_NT_HEADERS pntheader = (PIMAGE_NT_HEADERS)((BYTE*)pdosHeader + pdosHeader->e_lfanew);
	if (*((DWORD*)pntheader) != IMAGE_NT_SIGNATURE) {
		printf("此文件不是PE格式文件 或文件已损坏!!");
		fclose((FILE*)pImageBuffer);
	}
	PIMAGE_FILE_HEADER ppeheader = (PIMAGE_FILE_HEADER) & (pntheader->FileHeader);
	PIMAGE_OPTIONAL_HEADER poptionHeader = (PIMAGE_OPTIONAL_HEADER) & (pntheader->OptionalHeader);
	PIMAGE_SECTION_HEADER psectionHeader = (PIMAGE_SECTION_HEADER)((BYTE*)poptionHeader + ppeheader->SizeOfOptionalHeader);
	//printf("%x", psectionHeader->Misc);
	//定位pFileBuffer节区位置
	PIMAGE_SECTION_HEADER pfileBufferpsp = (PIMAGE_SECTION_HEADER)((BYTE*)psectionHeader - (BYTE*)pImageBuffer + (BYTE*)pFileBuffer);
	//判断pFileBuffer 文件节区内空白空间是否足够添加代码
	if (pfileBufferpsp->Misc.VirtualSize > pfileBufferpsp->SizeOfRawData) {
		printf("节区无可用空闲内存不可添加!!!!");
	}if (pfileBufferpsp->SizeOfRawData-pfileBufferpsp->Misc.VirtualSize< Codesize) {
		printf("节区内存空闲区长度不够添加该代码!!!!");
	}
	
	PBYTE Codbigin = (PBYTE)((DWORD)pImageBuffer + psectionHeader -> VirtualAddress+psectionHeader -> Misc.VirtualSize);
	printf("%x+%x=%x\n", pImageBuffer, psectionHeader -> VirtualAddress + psectionHeader -> Misc.VirtualSize,Codbigin);
	memcpy(Codbigin, ShellCode, Codesize);
	DWORD calladdress = (Maddress - (((DWORD)(Codbigin + 0xd) - (DWORD)pImageBuffer) + poptionHeader -> ImageBase));
	*(PDWORD)(Codbigin + 0x9) = calladdress;
	// 我写的
	// DWORD jmpsddress = (poptionHeader->AddressOfEntryPoint - (((DWORD)(Codbigin + Codesize - 1) - (DWORD)pImageBuffer) + poptionHeader->ImageBase));
	// 我抄的
	DWORD jmpsddress = ((poptionHeader -> ImageBase + poptionHeader -> AddressOfEntryPoint) - (poptionHeader -> ImageBase + ((DWORD)Codbigin + Codesize - (DWORD)pImageBuffer)));
	*(PDWORD)(Codbigin + 0xe) = jmpsddress;
	// poptionHeader -> AddressOfEntryPoint = (((DWORD)Codbigin - (DWORD)pImageBuffer - psectionHeader -> Misc.VirtualSize));(更改)
	poptionHeader -> AddressOfEntryPoint = (((DWORD)Codbigin - (DWORD)pImageBuffer));
	//printf("%x-%x=%x\n",(DWORD) Codbigin, (DWORD)pImageBuffer, poptionHeader->AddressOfEntryPoint);
	//把拉伸后的文件 在次收缩为文件类型
	PVOID pNewBuffer = 0;
	DWORD file_size = ImageBufferToNewBuffer(IN  pImageBuffer, OUT  &pNewBuffer);
	//文件存盘
	WriteMemeryToFile(IN  pNewBuffer, IN  file_size);
	free(pFileBuffer);
	free(pImageBuffer);
	free(pNewBuffer);

}
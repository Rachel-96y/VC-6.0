// Other_Person_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// readFile.cpp : 定义控制台应用程序的入口点。
//


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

DWORD fileSize = 0;

char f[80] = {0};
char write_file_path[80] = {0};


int FOAToRVA(PIMAGE_SECTION_HEADER pSectionHeader, int HeaderSize, int FOAaddress)
{
	int N = 0;
	int Count = 0;
	int RVAaddress = 0;
	int sectionNum = 0;
	int VirtualAddress = 0;
	PIMAGE_SECTION_HEADER pTemSectionHeader = pSectionHeader;
	int sizeOfRaw = 0;
	int actuallySize = 0;
	int pointerOfRaw = 0;
	int deviationAddress = 0;

	//D:\i4Tools7\avfilter-6.dll
	//遍历节
	if(FOAaddress == 0){return 0;}
	else if(FOAaddress <= HeaderSize){return FOAaddress;}
	else
	{
		while(pTemSectionHeader -> Characteristics != 0)
		{
			sectionNum++;

			sizeOfRaw = pTemSectionHeader -> SizeOfRawData;
			actuallySize = pTemSectionHeader -> Misc.VirtualSize;
			pointerOfRaw = pTemSectionHeader -> PointerToRawData;
			VirtualAddress = pTemSectionHeader -> VirtualAddress;
			sizeOfRaw > actuallySize ? N = sizeOfRaw : N = actuallySize;

			//1、用pointerOfRaw + sizeOfRaw，获取这个节的范围
			//2、用FOA和这个范围比较,确定所在节
			if(pointerOfRaw <= FOAaddress && FOAaddress <= (pointerOfRaw+sizeOfRaw))
			{
				/*
				Count = 1;
				printf("FOA所在节: %d\n", sectionNum);
				printf("FOAaddress: 0x%X\n", FOAaddress);
				printf("pointerOfRaw: 0x%X\n", pointerOfRaw);
				printf("pointerOfRaw+sizeOfRaw: 0x%X\n\n", pointerOfRaw+sizeOfRaw);
				*/

				//3、用FOA - 该节的PointToRawData = RVA偏移
			    deviationAddress = FOAaddress - pointerOfRaw;

				//4、RVAaddress = VirtualAddress + RVA偏移
				RVAaddress = VirtualAddress + deviationAddress;
				//5、判断是否 VirtualAddress <= RVA < (VirtualAddress + N) 
				
				if(VirtualAddress <= RVAaddress && RVAaddress < (VirtualAddress + N))
				{
					return RVAaddress;
				}
			}
			// if(Count == 1){break;}
	
			pTemSectionHeader = (PIMAGE_SECTION_HEADER)((int)pTemSectionHeader + 40);
		}
	}
	return NULL;
}


int RVAToFOA(PIMAGE_SECTION_HEADER pSectionHeader, int HeaderSize, int RVAaddress)
{
	int N = 0;
	int Count = 0;
	int FOAaddress = 0;
	int sectionNum = 0;
	int VirtualAddress = 0;
	PIMAGE_SECTION_HEADER pTemSectionHeader = pSectionHeader;
	int sizeOfRaw = 0;
	int actuallySize = 0;


	//遍历节
	if(RVAaddress == 0){return 0;}
	else if(RVAaddress <= HeaderSize){return RVAaddress;}
	else
	{
		while(pTemSectionHeader -> Characteristics != 0)
		{
			sectionNum++;
			//1、对比SizeOfRawData和Misc的值,谁大取谁

			sizeOfRaw = pTemSectionHeader -> SizeOfRawData;
			actuallySize = pTemSectionHeader -> Misc.VirtualSize;
			sizeOfRaw > actuallySize ? N=sizeOfRaw : N = actuallySize;
			VirtualAddress = pTemSectionHeader -> VirtualAddress;


			//2、用VirtualAddress + N，获取这个节的范围
			//3、用RVA和这个范围比较,确定所在节
			if(VirtualAddress <= RVAaddress && RVAaddress <= (VirtualAddress + N))
			{
				/*
				Count = 1;
				printf("RVA所在节: %d\n", sectionNum);
				printf("RVAaddress: 0x%X\n", RVAaddress);
				printf("VirtualAddress: 0x%X\n", VirtualAddress);
				printf("VirtualAddress + N: 0x%X\n\n", VirtualAddress + N);
				*/
				
				//4、获取在镜像中的偏移位置
				int deviationAddress = RVAaddress - VirtualAddress;
			
				//5、用该节的PointToRawData + RVA的偏移，即RVA的FOA
				FOAaddress = pTemSectionHeader -> PointerToRawData + deviationAddress;
				return FOAaddress;
			}
			//if(Count == 1){break;}

			//不存在，就跳到下一个节
			pTemSectionHeader = (PIMAGE_SECTION_HEADER)((int)pTemSectionHeader + 40);
		}
	}
	return NULL;
}

void tansformTimeStamp(time_t time)
{
	time_t PTime = 0;
    struct tm* timeP;

    PTime = time + (8 * 60 *60);
    timeP = localtime(&PTime);    // 转换
    printf("%d-%d-%d %d:%d:%d\n",1900 + timeP -> tm_year, 1 + timeP -> tm_mon, timeP -> tm_mday, \
		timeP -> tm_hour, timeP -> tm_min, timeP -> tm_sec);
}

int saveFile(PVOID NewFileBuffer, DWORD FileSize, char* FilePath)
{
	FILE* fp1 = fopen(FilePath, "wb");
	if(fp1 != NULL)
	{
		fwrite(NewFileBuffer, FileSize, 1, fp1);
	}
	fclose(fp1);
	return 1;

}

LPVOID readPEFile(LPSTR filePath)
{
	//定义文件指针
	FILE *pFile = NULL;
	
	LPVOID pFileBuffer = NULL;

	//打开文件
	pFile = fopen(filePath, "rb");
	if(!pFile)
	{
		printf("无法打开该程序！");
		return NULL;
	}

	//将指针调到文件末尾
	fseek(pFile, 0, SEEK_END);
	//读取文件大小
	fileSize = ftell(pFile);
	
	//将指针调到文件开头
	fseek(pFile, 0, SEEK_SET);
	//分配缓存区
	pFileBuffer = malloc(fileSize);
	if(!pFileBuffer)
	{
		printf("内存分配失败！");
		free(pFileBuffer);
		pFileBuffer = NULL;
		fclose(pFile);
		return NULL;
	}
	size_t flag = fread(pFileBuffer, fileSize, 1, pFile);
	if(!flag)
	{
		printf("数据读取失败！");
		free(pFileBuffer);
		pFileBuffer = NULL;
		fclose(pFile);
		return NULL;
	}
	
	//读取成功后关闭文件
	fclose(pFile);
	return pFileBuffer;
}

//计算对齐后的大小
int countAlignment(int virtualSize, int alignmentNum)
{
	if(virtualSize % alignmentNum == 0)
	{
		return virtualSize;
	}else
	{
		return ((virtualSize/alignmentNum) + 1) * alignmentNum;
	}
}

//把文件缓冲 读取到 镜像缓冲区中
//参数：文件指针、镜像大小、头大小、DOS头指针、节数、节指针
DWORD CopyFileBufferToImageBuffer(IN LPVOID pFileBuffer, DWORD SizeOfImage, DWORD SizeOfHeaders, PIMAGE_DOS_HEADER pDOSHeader, \
								  DWORD NumberOfSection, PIMAGE_SECTION_HEADER pSectionHeader, OUT LPVOID* pImage)
{
	LPVOID pImageBuffer = malloc(SizeOfImage);
	//根据镜像大小分配空间
	if(!pImageBuffer)
	{
		printf("内存分配失败！");
		return 0;
	}
	//初始化缓冲区,从头开始赋0
	memset(pImageBuffer, 0, SizeOfImage); 
	//先把头放入缓存区中
	memcpy(pImageBuffer, pDOSHeader, SizeOfHeaders);
	//根据节表多少，循环录入，从磁盘中节开始的地方加上磁盘中对齐后的大小
	for(UINT i = 0; i<NumberOfSection; i++, pSectionHeader++)
	{
		memcpy((LPVOID)((DWORD)pImageBuffer + pSectionHeader -> VirtualAddress), \
			(LPVOID)((DWORD)pDOSHeader + pSectionHeader -> PointerToRawData), pSectionHeader -> SizeOfRawData);
	}

	*pImage = pImageBuffer;
	pImageBuffer = NULL;
	return SizeOfImage;
}

void printHeaders()
{
	LPVOID pFileBuffer = NULL;
	PIMAGE_DOS_HEADER pDOSHeader = NULL;
	PIMAGE_NT_HEADERS pNTHeaders = NULL;
	PIMAGE_FILE_HEADER pPEHeader = NULL;
	PIMAGE_OPTIONAL_HEADER32 pOptionHeader = NULL;
	PIMAGE_SECTION_HEADER  pSectionHeader = NULL;
	PIMAGE_EXPORT_DIRECTORY pExportTable = NULL;
	PIMAGE_IMPORT_DESCRIPTOR pImportTable = NULL;
	PIMAGE_BOUND_IMPORT_DESCRIPTOR pImportBondTable = NULL;
	PIMAGE_BOUND_FORWARDER_REF pBondRef = NULL;
	int* functionAddress = NULL;
	short* orderAddress = NULL;
	int* functionName = NULL;
	char* pName = NULL;

	

	pFileBuffer = readPEFile(f);
	if(!pFileBuffer)
	{
		printf("无法打开该程序！");
		return;
	}

	//判断是否有MZ标志
	if(*(PWORD)pFileBuffer != IMAGE_DOS_SIGNATURE)
	{
		printf("0x%X\n", *(PWORD)pFileBuffer);
		printf("不存在MZ标识，请确认打开的文件是否为.exe .dll .sys文件");
		//释放堆内存	
		free(pFileBuffer);
		pFileBuffer = NULL;
		return;
	}

	printf("\n\n");

	pDOSHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	printf("MZ标识: %c%c\n", pDOSHeader -> e_magic, *((char*)pFileBuffer+1));
	printf("PE偏移: 0x%X\n", pDOSHeader -> e_lfanew);
	if(*((PWORD)((DWORD)pFileBuffer+pDOSHeader -> e_lfanew)) != IMAGE_NT_SIGNATURE)
	{
		printf("不存在PE标识,文件可能已损坏");
		//释放堆内存	
		free(pFileBuffer);
		pFileBuffer = NULL;
		return;
	}

	pNTHeaders = (PIMAGE_NT_HEADERS)((DWORD)pFileBuffer+pDOSHeader -> e_lfanew);
	printf("NT头: %c%c\n", pNTHeaders -> Signature, *((char*)(pDOSHeader -> e_lfanew + (DWORD)pFileBuffer + 1)));
	pPEHeader = (PIMAGE_FILE_HEADER)(((DWORD)pNTHeaders) + 4);
	pOptionHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pPEHeader + IMAGE_SIZEOF_FILE_HEADER);


	int HeaderSize = pOptionHeader -> SizeOfHeaders;
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionHeader + pPEHeader -> SizeOfOptionalHeader);


	int sectionStartingPosition = pDOSHeader -> e_lfanew+24 + pPEHeader -> SizeOfOptionalHeader;
	int sectionNum = 0;
	
	printf("=======================<<节表>>=======================\n\n");
	while((pSectionHeader -> Characteristics != 0) && (sectionNum < (pPEHeader -> NumberOfSections))){
		sectionNum++;
		pSectionHeader++; //获取最后一个节的地址
	}
	char* mark = (char*)pSectionHeader;
	int count = 0;
	int flag = 1;
	
	for(int i = 0; i < 80; i++)
	{
		if(*mark != 0)
		{
			printf("最后一节后有数据.空间不足够：%d\n", sectionNum);
			flag = 0;
			break;
		}else{
			count++;
			mark++;
		}
	}
	if(count == 80)printf("空间足够,可以新增节表\n\n");
	
	LPVOID pNewFileBuffer = NULL;
	PIMAGE_SECTION_HEADER pTemSectionHeader = NULL;
	PIMAGE_SECTION_HEADER pPreSectionHeader = NULL;
	PIMAGE_DOS_HEADER pNewDOSHeader = NULL;
	int SizeOfNewFileBuffer = 0;
	int virtualSize = 0x1200;
	

	printf("节表结束位置: 0x%X\n", (int)pSectionHeader - (int)pFileBuffer);
	if(flag == 1)
	{	
		pPreSectionHeader = pSectionHeader - 1;


		//节名
		strcpy((char*)pSectionHeader -> Name, (char*)".NewSec");
		pSectionHeader -> Misc.VirtualSize = virtualSize;
		pSectionHeader -> VirtualAddress = countAlignment(((pPreSectionHeader -> VirtualAddress) + \
			(pPreSectionHeader -> Misc.VirtualSize)), pOptionHeader -> SectionAlignment);
		pSectionHeader -> SizeOfRawData = countAlignment(pSectionHeader -> Misc.VirtualSize, pOptionHeader -> FileAlignment);
		pSectionHeader -> PointerToRawData = pPreSectionHeader -> PointerToRawData + pPreSectionHeader -> SizeOfRawData;
		pSectionHeader -> Characteristics = 0xE0; 
		pPEHeader -> NumberOfSections = sectionNum + 1;


		//修改镜像大小
		pOptionHeader -> SizeOfImage = pSectionHeader -> VirtualAddress + countAlignment(pSectionHeader -> Misc.VirtualSize, pOptionHeader -> SectionAlignment);
		

		printf("对齐后的节大小: 0x%X\n\n", countAlignment(pSectionHeader -> Misc.VirtualSize, pOptionHeader -> FileAlignment));

		SizeOfNewFileBuffer = (int)(pPreSectionHeader -> PointerToRawData + pPreSectionHeader -> SizeOfRawData) + pSectionHeader -> SizeOfRawData;
		
		pNewFileBuffer = malloc(SizeOfNewFileBuffer);
		if(pNewFileBuffer == NULL)
		{
			printf("新建缓冲区失败\n\n");
			return;
		}

		//新节区之前的值全部复制，新节表部分已改变
		memcpy(pNewFileBuffer, pFileBuffer, (int)(pPreSectionHeader -> PointerToRawData + pPreSectionHeader -> SizeOfRawData));
 
		//新节区全部置零
		memset((void *)((DWORD)pNewFileBuffer + pSectionHeader -> PointerToRawData), 0, pSectionHeader -> SizeOfRawData);
		

	}else
	{
		//空间不足往上提升
		//1、新建的缓存区
		int sizeOfRawData = 0;
		pPreSectionHeader = pSectionHeader - 1;
		sizeOfRawData = countAlignment(virtualSize,pOptionHeader -> FileAlignment);
		SizeOfNewFileBuffer = (int)(pPreSectionHeader -> PointerToRawData + pPreSectionHeader -> SizeOfRawData) + sizeOfRawData;
		printf("新文件所需空间: 0x%X\n\n", SizeOfNewFileBuffer);

		//新节区全部置零
		pNewFileBuffer = malloc(SizeOfNewFileBuffer);
		memset(pNewFileBuffer, 0, SizeOfNewFileBuffer);
		if(pNewFileBuffer == NULL)
		{
			printf("新建缓冲区失败\n\n");
			return ;
		}

		//2、复制DOS头
		memcpy(pNewFileBuffer, pFileBuffer, 0x40);
		pNewDOSHeader = (PIMAGE_DOS_HEADER)((int)pNewFileBuffer + 0x40);


		//3、从NT头复制到最后一个节表
		pPEHeader -> NumberOfSections = sectionNum + 1;
		memmove(pNewDOSHeader, pNTHeaders, (int)pSectionHeader - (int)pNTHeaders);


		//算出插入位置
		int insertAddress = ((int)pSectionHeader - (int)pNTHeaders)+ 0x40;
		pTemSectionHeader = (PIMAGE_SECTION_HEADER)((int)pNewFileBuffer + insertAddress);
		pPreSectionHeader = pTemSectionHeader - 1;

		//printf("pSectionHeader - pNTHeaders: 0x%X\n", (int)pSectionHeader - (int)pNTHeaders);
		//printf("文件中节表插入位置: 0x%X\n", insertAddress);

		//4、新增节表
		strcpy((char*)pTemSectionHeader -> Name, (char*)".NewSec");
		pTemSectionHeader -> Misc.VirtualSize = virtualSize;
		pTemSectionHeader -> VirtualAddress = countAlignment(((pPreSectionHeader -> VirtualAddress) + (pPreSectionHeader -> Misc.VirtualSize)), pOptionHeader -> SectionAlignment);
		pTemSectionHeader -> SizeOfRawData = countAlignment(pTemSectionHeader -> Misc.VirtualSize, pOptionHeader -> FileAlignment);
		pTemSectionHeader -> PointerToRawData = pPreSectionHeader -> PointerToRawData + pPreSectionHeader -> SizeOfRawData;
		pTemSectionHeader -> Characteristics = 0xE0; 
		((PIMAGE_DOS_HEADER)pNewFileBuffer) -> e_lfanew = 0x40;



		//修改镜像大小
		pOptionHeader = (PIMAGE_OPTIONAL_HEADER32)((int)pNewDOSHeader + 0x18);
		printf("先前的SizeOfImage: 0x%X\n", pOptionHeader -> SizeOfImage);
		pOptionHeader -> SizeOfImage = pTemSectionHeader -> VirtualAddress + countAlignment(pTemSectionHeader -> Misc.VirtualSize,pOptionHeader -> SectionAlignment);
		printf("pOptionHeader的偏移: 0x%X\n\n", (int)pOptionHeader - (int)pNewFileBuffer);
		printf("最后一节的VirtualAddress: 0x%X    以及对齐后的节大小: 0x%X   最终的SizeOfImage: 0x%X\n\n", \
			pTemSectionHeader -> VirtualAddress, countAlignment(pTemSectionHeader -> Misc.VirtualSize, pOptionHeader -> SectionAlignment), pOptionHeader -> SizeOfImage);



		//5、复制剩余内容
		int offset = (int)pSectionHeader - (int)pFileBuffer; //节表末位置偏移
		memmove((LPVOID)((int)pNewFileBuffer+offset), pSectionHeader, (pPreSectionHeader -> PointerToRawData + pPreSectionHeader -> SizeOfRawData) - offset);

		sectionStartingPosition = ((int)pNewDOSHeader + 0x18 + pPEHeader -> SizeOfOptionalHeader) - (int)pNewFileBuffer;
		pSectionHeader = pTemSectionHeader;
	}

	
	printf("插入节表名称: .NewSec\n\n");
	printf("节表的起始位置: 0x%X\n\n", sectionStartingPosition);
	printf("节的实际大小: 0x%X\n", pSectionHeader -> Misc.VirtualSize);
	printf("该节在内存中的偏移地址: 0x%X\n", pSectionHeader -> VirtualAddress);
	printf("该节在磁盘中的偏移地址: 0x%X\n", pSectionHeader -> PointerToRawData);
	printf("该节在磁盘中对齐后的大小: 0x%X\n", pSectionHeader -> SizeOfRawData);
	printf("该节在磁盘中结束位置: 0x%X\n\n", (pSectionHeader -> PointerToRawData + pSectionHeader -> SizeOfRawData) - 16);
	printf("原文件中复制截止位置: 0x%X\n\n", (int)(pPreSectionHeader -> PointerToRawData + pPreSectionHeader -> SizeOfRawData));

	//===========================新增节结束===========================

	
	PIMAGE_SECTION_HEADER pNewSecTable = (PIMAGE_SECTION_HEADER)(sectionStartingPosition + (DWORD)pNewFileBuffer);						//newBuffer中第一张节表的地址
	PIMAGE_SECTION_HEADER pNewEndSecTable = (PIMAGE_SECTION_HEADER)((int)pNewSecTable + ((pPEHeader -> NumberOfSections) - 1) * 40);	//newBuffer中最后一张张节表的地址
	PIMAGE_OPTIONAL_HEADER32 pOpNewtionHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pNewFileBuffer+pDOSHeader -> e_lfanew + 24);			//newBuffer中的可选PE头
	LPVOID pNewSecStarPosition = NULL;

	int funAddressRVA = 0;
	int funOrderRVA = 0;
	int funNameRVA = 0;
	int exportTableRVA = 0;

	int* pTempFunctionName = NULL;
	int* dllName = NULL;


	//1、获取导出表以及其内部信息
	//导出表
	//如果导出表存在就打印导出表
	if(((pOpNewtionHeader -> DataDirectory[0].VirtualAddress) != 0) && ((pOpNewtionHeader -> DataDirectory[0].VirtualAddress) != 0x1000))
	{
		//把磁盘中表的位置赋值给指针
		pExportTable = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pNewFileBuffer + (RVAToFOA(pNewSecTable, HeaderSize, pOpNewtionHeader -> DataDirectory[0].VirtualAddress)));

		printf("导出表RVA: 0x%X\n", pOpNewtionHeader -> DataDirectory[0].VirtualAddress);
		printf("导出表FOA: 0x%X\n\n", (RVAToFOA(pNewSecTable, HeaderSize, pOpNewtionHeader -> DataDirectory[0].VirtualAddress)));

		pName = (char*)((pExportTable -> Name + (DWORD)pNewFileBuffer) + 1);

		if(RVAToFOA(pNewSecTable, HeaderSize, pExportTable -> Name) < (int)fileSize)
		{
			pName = (char*)( RVAToFOA(pNewSecTable, HeaderSize, pExportTable -> Name) + (DWORD)pNewFileBuffer);
			printf("导出表名字: ");

					while(*(pName) != 0)
					{
						printf("%c", *pName);
						pName++;
					}
					printf("\n");

		}else{printf("导出表信息有误！\n\n");}


		functionName = (int*)((DWORD)pNewFileBuffer + (RVAToFOA(pNewSecTable, HeaderSize, pExportTable -> AddressOfNames)));            //函数名字表地址
		orderAddress = (short*)((DWORD)pNewFileBuffer + (RVAToFOA(pNewSecTable, HeaderSize, pExportTable -> AddressOfNameOrdinals)));   //序号表地址
		functionAddress = (int*)((DWORD)pNewFileBuffer + (RVAToFOA(pNewSecTable, HeaderSize, pExportTable -> AddressOfFunctions)));	  //函数表地址
		int* pTemfunctionName = functionName;
		short* pTemOrderAddress = orderAddress;
		pTempFunctionName = functionName;		//固定值

		int* tempFunctionName = functionName;

		printf("原文件大小: 0x%X  %dKB\n", fileSize, fileSize / 1024);

		//修改文件大小，此时是新增节后的大小
		fileSize = pNewEndSecTable -> PointerToRawData + pNewEndSecTable -> SizeOfRawData;
		printf("新增节后文件大小: 0x%X  %dKB\n\n", fileSize, fileSize / 1024);

		//新节起始位置就是NewFileBuffer+新节在内存中的偏移	
		LPVOID pNewSecStarPosition = (LPVOID)((DWORD)pNewFileBuffer + pNewEndSecTable -> PointerToRawData);


		//2、根据函数表大小移动函数表  大小=NumberOfFunctions*4  移动到新增节起始位置,并把指针指向末尾
		memmove(pNewSecStarPosition, functionAddress, ((pExportTable -> NumberOfFunctions) * 4));
		funAddressRVA = FOAToRVA(pNewSecTable, HeaderSize, (int)pNewSecStarPosition - (int)pNewFileBuffer);
		printf("移动后函数地址表的FOA: 0x%X\n", (int)pNewSecStarPosition - (int)pNewFileBuffer);
		pNewSecStarPosition = (LPVOID)((int)pNewSecStarPosition + (pExportTable -> NumberOfFunctions) * 4);
		


		//3、在函数表后面，放入序号表  大小=NumberOfNames*2   并把指针指向末尾
		memmove(pNewSecStarPosition ,orderAddress, ((pExportTable -> NumberOfNames) * 2));
		funOrderRVA = FOAToRVA(pNewSecTable, HeaderSize, (int)pNewSecStarPosition - (int)pNewFileBuffer);
		printf("移动后序号表的FOA: 0x%X\n", (int)pNewSecStarPosition - (int)pNewFileBuffer);
		pNewSecStarPosition = (LPVOID)((int)pNewSecStarPosition + (pExportTable -> NumberOfNames) * 2);
		


		//4、移动函数名称，放入序号称表后面 之后替换函数名称表中的函数名称RVA
		for(int i = 0; i < (int)pExportTable -> NumberOfFunctions; i++)
		{
			//匹配函数名称表
			for(int k = 0; k < (int)pExportTable -> NumberOfNames; k++)
			{
					
				if((*orderAddress) == k)
				{
			
					if(RVAToFOA(pNewSecTable, HeaderSize, *functionName) < (int)fileSize)
					{		
						//名字的起始地址RVA
						pName = (char*)(RVAToFOA(pNewSecTable, HeaderSize, *functionName) + (DWORD)pNewFileBuffer);

						//获取复制地址的RVA
						funNameRVA = FOAToRVA(pNewSecTable, HeaderSize, (int)pNewSecStarPosition - (int)pNewFileBuffer);

						while(*(pName) != 0)
						{	
							//把名字一个个复制过去
							memmove(pNewSecStarPosition, pName, 1);
							pNewSecStarPosition = (LPVOID)((int)pNewSecStarPosition + 1);
							pName++;
						}
						
						//名字复制完毕后，把名字的RVA放到函数名称表中
						*functionName = funNameRVA;
						pNewSecStarPosition = (LPVOID)((int)pNewSecStarPosition + 1);
							
					}

					functionName++;
					break;

				}else{
					tempFunctionName++;
				}
			}

			functionAddress++;
		}

		//5、移动导出表函数名
		if(RVAToFOA(pNewSecTable, HeaderSize, pExportTable -> Name) < (int)fileSize)
		{
			pName = (char*)( RVAToFOA(pNewSecTable, HeaderSize, pExportTable -> Name) + (DWORD)pNewFileBuffer );
			dllName =(int*)(FOAToRVA(pNewSecTable, HeaderSize, (int)pNewSecStarPosition - (int)pNewFileBuffer));

			while(*(pName) != 0)
			{
				memmove(pNewSecStarPosition, pName, 1);
				pNewSecStarPosition = (LPVOID)((int)pNewSecStarPosition + 1);
				pName++;
			}
			pNewSecStarPosition = (LPVOID)((int)pNewSecStarPosition + 1);

		}else{printf("导出表信息有误！\n\n");}

		//6、修改导出表中导出表名字的偏移
		pExportTable -> Name = (int)dllName;

		//7、移动函数名称表，移动到函数名字后面  大小=NumberOfNames*4
		memmove(pNewSecStarPosition, pTempFunctionName, (pExportTable -> NumberOfNames) * 4);
		funNameRVA = FOAToRVA(pNewSecTable, HeaderSize, (int)pNewSecStarPosition - (int)pNewFileBuffer);
		printf("移动后函数名称表: 0x%X\n\n", (int)pNewSecStarPosition - (int)pNewFileBuffer);
		pNewSecStarPosition = (LPVOID)((int)pNewSecStarPosition + (pExportTable -> NumberOfNames) * 4);
		

		//8、修改移动后导出表中的 函数表RVA、序号表RVA、名字表RVA
		pExportTable -> AddressOfFunctions = funAddressRVA;
		pExportTable -> AddressOfNameOrdinals = funOrderRVA;
		pExportTable -> AddressOfNames = funNameRVA;

		//9、最后把导出表移动到函数名称表后面
		memmove(pNewSecStarPosition, pExportTable, 0x28);
		exportTableRVA = FOAToRVA(pNewSecTable, HeaderSize, (int)pNewSecStarPosition - (int)pNewFileBuffer);

		//10、修改目录项中导出表的RVA
		pOpNewtionHeader -> DataDirectory[0].VirtualAddress = exportTableRVA;

		saveFile(pNewFileBuffer, SizeOfNewFileBuffer, write_file_path);
		printf("存盘成功！\n\n");

	}else{printf("此文件没有导出表\n\n");}




	//释放指针
	pOpNewtionHeader = NULL;
	pNewSecTable = NULL;
	pNewEndSecTable = NULL;
	pNewSecStarPosition = NULL;
	pTemSectionHeader = NULL;
	pPreSectionHeader = NULL;
    	pNewFileBuffer = NULL;
	pDOSHeader = NULL;
	pNTHeaders = NULL;
	pPEHeader = NULL;
	pOptionHeader = NULL;
	pSectionHeader = NULL;
	pExportTable = NULL;
	pImportTable = NULL;
	pImportBondTable = NULL;
	pBondRef = NULL;
	functionAddress = NULL;
	orderAddress = NULL;
	functionName = NULL;
	pName = NULL;

	free(pFileBuffer);
	pFileBuffer = NULL;

}

VOID SetNewFileName(IN LPSTR OldFilePath)
{
	/* 
		根据传入的文件路径 
		将原路径更改位一个新的文件路径(仅更改文件名)
	*/

	if (!OldFilePath)
	{
		// 值为空则返回
		return;
	}

	DWORD strLength = strlen(OldFilePath);
	LPSTR pOldFileName = NULL;

	// 找到最后一个 '\' 的后一个字节的位置
	for (UINT i = 0; i < strLength; i++)
	{
		if (*OldFilePath++ == '\\')
		{
			pOldFileName = OldFilePath;
		}
	}
	
	char OldFileName[80] = {0};
	// 保存旧的文件名到 OldFileName 中
	strcpy(OldFileName, pOldFileName);
	
	// 添加字符串 'New' 到 '\' 后
	strcpy(pOldFileName, "New");
	pOldFileName += 3;
	
	// 拼接得到完整路径
	strcpy(pOldFileName, OldFileName);

	return;
}

int main(int argc, char* argv[])
{
	printf("请输入不包含空格的文件路径: \n");
	scanf("%s", f);
 	strcpy(write_file_path, f);
	SetNewFileName(write_file_path);
	
	printHeaders();

	system("pause");
	return 0;
}





//头大小小于内存对齐时，节就从 内存对齐大小开始
//当头大小超过内存对齐时，用头大小%内存大小，(结果 + 1) * 内存大小）
//接下来判断Misc，Misc的值的大小是否超过内存对齐，接下来干的和上面一样
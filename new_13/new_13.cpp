#include "stdafx.h"

int function(int x, int y)
{
	return x + y;
}

unsigned char Array[] = 
{
	0x55,
	0x8B, 0xEC,
	0x83, 0xEC, 0x40,
	0x53,
	0x56,
	0x57,
	0x8D, 0x7D, 0xC0,
	0xB9, 0x10, 0x00, 0x00, 0x00,
	0xB8, 0xCC, 0xCC, 0xCC, 0xCC,
	0xF3, 0xAB,
	0x8B, 0x45, 0x08,
	0x03, 0x45, 0x0C,
	0x5F,
	0x5E,
	0x5B,
	0x8B, 0xE5,
	0x5D,
	0xC3,
};

char arr[] = 
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x09,
	0x00,0x20,0x10,0x03,0x03,0x0C,0x00,0x00,0x44,0x00,
	0x00,0x33,0x00,0x47,0x0C,0x0E,0x00,0x0D,0x00,0x11,
	0x00,0x00,0x00,0x02,0x64,0x00,0x00,0x00,0x7F,0x00,	
	0x00,0x00,0x64,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x00,0x74,0x0F,0x41,0x00,0x00,0x00,
	0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0A,0x00,
	0x00,0x02,0x74,0x0F,0x41,0x00,0x06,0x08,0x00,0x00,
	0x00,0x00,0x00,0x64,0x00,0x0F,0x00,0x00,0x0D,0x00,
	0x00,0x00,0x23,0x00,0x00,0x64,0x00,0x00,0x64,0x00,
};
		
/*
3��char�����������£�					
					
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x09,					
0x00,0x20,0x10,0x03,0x03,0x0C,0x00,0x00,0x44,0x00,					
0x00,0x33,0x00,0x47,0x0C,0x0E,0x00,0x0D,0x00,0x11,					
0x00,0x00,0x00,0x02,0x64,0x00,0x00,0x00,0xAA,0x00,					
0x00,0x00,0x64,0x10,0x00,0x00,0x00,0x00,0x00,0x00,					
0x00,0x00,0x02,0x00,0x74,0x0F,0x41,0x00,0x00,0x00,					
0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0A,0x00,					
0x00,0x02,0x74,0x0F,0x41,0x00,0x06,0x08,0x00,0x00,					
0x00,0x00,0x00,0x64,0x00,0x0F,0x00,0x00,0x0D,0x00,					
0x00,0x00,0x23,0x00,0x00,0x64,0x00,0x00,0x64,0x00					
					
ָ�붨�����£�			������˵������Ľ����		
					
int (*px)[2]; 			     *(*(px+0)+0)		
			                 0x03020100		
int (*py)[2][3]; 			 *(*(px+1)+0)		
			                 0x20000907		
char (*pz)[2];			     *(*(px+2)+3)		
			                 0x00001100		
char (*pk)[2][3];			 *(*(*(py+1)+2)+3)		
			                 0x00000001		
							 *(*(pz+2)+3)		
							 0x07		
							 *(*(*(pk+2)+3)+4)		
							 0x0E		
*/

int main(int argc, char* argv[])
{
	// ��һ�������洢����������ͨ��ָ����з���
	typedef int(__cdecl *typeArray)(int, int);
	typeArray pArray = (int(__cdecl *)(int, int))&Array;
	// int(__cdecl *pArray)(int, int) = (int(__cdecl *)(int, int))&Array;  ������������ֵ
	printf("%d\n",pArray(1, 2));  // ����ָ�� ָ������
	printf("%d\n",function(3, 4));   // ������������
	typeArray pFunction = (int(__cdecl *)(int, int))&function;
	printf("%d\n",pFunction(5, 6));  // ����ָ�� ָ����


	// ������˵������ļ��
	int (*px)[2] = (int(*)[2])&arr[0];
	printf("%x\n", *(*(px+0)+0));
	printf("%x\n", *(*(px+1)+0));
	printf("%x\n", *(*(px+2)+3));
	int (*py)[2][3] = (int(*)[2][3])&arr[0];
	printf("%x\n", *(*(*(py+1)+2)+3));
	char (*pz)[2] = (char(*)[2])&arr[0];
	printf("%x\n", *(*(pz+2)+3));
	char (*pk)[2][3] =(char(*)[2][3])&arr[0];
	printf("%x\n", *(*(*(pk+2)+3)+4));
	getchar();

	return 0;
}
/*
本节练习：							
							
1、创建一个int* arr[5] 数组，并为数组赋值(使用&,不能使用强制转型).							
							
2、创建一个字符指针数组，存储所有的C的关键词(查资料找)，并全部打印出来.							
							
3、查找这些数据中，有几个 id == 1 level == 8 的结构体信息.						
							
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x09,							
0x00,0x20,0x10,0x03,0x03,0x0C,0x00,0x00,0x44,0x00,							
0x00,0x33,0x01,0x00,0x00,0x08,0x00,0x00,0x00,0x00,							
0x00,0x00,0x00,0x02,0x64,0x00,0x00,0x00,0x7F,0x00,							
0x00,0x00,0x64,0x01,0x00,0x00,0x00,0x08,0x00,0x00,							
0x00,0x00,0x02,0x00,0x74,0x0F,0x41,0x00,0x00,0x00,							
0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0A,0x00,							
0x00,0x02,0x57,0x4F,0x57,0x00,0x06,0x08,0x00,0x00,							
0x00,0x00,0x00,0x64,0x00,0x0F,0x00,0x00,0x0D,0x00,							
0x00,0x00,0x23,0x00,0x00,0x64,0x00,0x00,0x64,0x00,							
							
结构体定义如下：							
							
typedef struct TagPlayer							
{							
	int id;						
	int level;						
}Player;							
*/

#include "stdafx.h"

typedef struct TagPlayer							
{							
	int id;						
	int level;						
}Player;

Player Example;

char Addr[100] = 
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x07,0x09,							
	0x00,0x20,0x10,0x03,0x03,0x0C,0x00,0x00,0x44,0x00,							
	0x00,0x33,0x01,0x00,0x00,0x08,0x00,0x00,0x00,0x00,							
	0x00,0x00,0x00,0x02,0x64,0x00,0x00,0x00,0x7F,0x00,							
	0x00,0x00,0x64,0x01,0x00,0x00,0x00,0x08,0x00,0x00,							
	0x00,0x00,0x02,0x00,0x74,0x0F,0x41,0x00,0x00,0x00,							
	0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0A,0x00,							
	0x00,0x02,0x57,0x4F,0x57,0x00,0x06,0x08,0x00,0x00,							
	0x00,0x00,0x00,0x64,0x00,0x0F,0x00,0x00,0x0D,0x00,							
	0x00,0x00,0x23,0x00,0x00,0x64,0x00,0x00,0x64,0x00,
};

int* arrx[5];
int arry[5] = {1, 2, 3, 4, 5};

void arrChange ()
{
	// 1、创建一个int* arr[5] 数组，并为数组赋值 (使用&,不能使用强制转型).
	for (int i = 0; i < 5; i++)
	{
		arrx[i] = &arry[i];
	}
}

void CharArr ()
{
	// 2、创建一个字符指针数组，存储所有的C的关键词(查资料找)，并全部打印出来.
	char* arr[] = 
	{
		"auto", "double", "int", "struct", "break", 

		"else", "long", "switch", "case", "enum", 

		"register", "typedef", "char", "extern","return", 

		"union", "const", "float", "short", "unsigned", 

		"continue", "for", "signed", "void", "default", 

		"goto", "sizeof", "volatile", "do", "if", 

		"while", "static",
	};
	for (int i = 0; i < 32; i++)
	{
		printf("%s\n", *(arr + i));
	}
	return;
}

void __stdcall FindPlayer (char* pAddr, Player* pExample)
{
	// 3、查找这些数据中，有几个id == 1 level == 8的结构体信息.	
	int sum = 0;
	for (int i = 0;i < 93; i++)
	{
		pExample = (Player*)(pAddr++);
		if (pExample->id == 1 && pExample->level == 8)
		{
			printf("数据首地址是: 0x%x\n", pExample);
			sum++;
		}	
	}
	printf ("有 %d 个 id == 1 level == 8 的结构体信息\n", sum);
	return;
}

int main (int argc, char* argv[])
{	
	// arrChange ();
	// CharArr ();
	// FindPlayer (&Addr[0], &Example);
	return 0;
}
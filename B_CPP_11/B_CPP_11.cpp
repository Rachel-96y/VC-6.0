// B_CPP_11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct MyStruct
{	
	int x;
	int y;
	
	MyStruct()
	{
	
	}

	MyStruct(int x, int y)
	{
		this -> x = x;
		this -> y = y;
	}

	~MyStruct()
	{
		printf("ÊÍ·ÅÁË!\n");
	}

};

void function()
{
	MyStruct* pA = new MyStruct(2, 3);
	printf("x: %d\ny: %d\n", pA -> x, pA -> y);
	delete pA;
	pA = NULL;

	MyStruct* pB = (MyStruct*)malloc(sizeof(MyStruct));
	pB -> x = 2;
	pB -> y = 3;
	printf("x: %d\ny: %d\n", pB -> x, pB -> y);
	free(pB);
	pB = NULL;

	int (*pi_1)[5] = new int[5][5];
	memset(pi_1, 0x00000000, sizeof(int) * 5 * 5);
	pi_1[4][4] = 0x12345678;
	printf("%x\n", *(*(pi_1 + 4) + 4));
	delete[] pi_1;
	pi_1 = NULL;

	int (*pi_2)[5] = (int (*)[5])malloc(sizeof(int) * 5 * 5);
	memset(pi_2, 0x00000000, sizeof(int) * 4 * 4);
	delete[] pi_2;
	pi_2 = NULL;

	LPVOID pMem_1 = malloc(sizeof(CHAR) * 10);
	memset(pMem_1, 0x55555555, sizeof(CHAR) * 10);

	char* pMem_2 = new char[10];
	memset(pMem_2, 0x55555555, sizeof(CHAR) * 10);
}

int main()
{
	function();

	getchar();
	return 0;
}
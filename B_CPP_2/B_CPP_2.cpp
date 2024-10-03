// B_CPP_2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef struct MyStructTest
{
	DWORD X;
	DWORD Y;

	void MyFunction(LPDWORD ChangePointer)
	{
		// 更改this指针的值
		LPDWORD pMyPointer = (LPDWORD)&pMyPointer;  
		*(pMyPointer + 1) = (UINT)ChangePointer;
		printf("%d\n%d\n", this -> X, this -> Y);
	}

} MST, *PMST;

int main(int argc, char* argv[])
{
	DWORD a = 1;
	DWORD b = 2;
	DWORD c = 3;
	MST mst;
	mst.X = 4;
	mst.Y = 5;
	mst.MyFunction(&c);
	getchar();

	return 0;
}

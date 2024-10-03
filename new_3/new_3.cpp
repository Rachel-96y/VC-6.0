#include "stdafx.h"
#include <windows.h>
#include <tchar.h>

void  func()
{
	int arr[] = {1, 2, 3, 4, 5, 7, 8};
	int* p = arr;
	int* q = p;

	*(q - 1) = *q;
	
	for (int i = 0; i < 4; i++)
	{
		*(q + i) = *(q + i + 1);
	}
	
	*(q + 4) = 6;

	for (int j = -1; j < 7; j++)
	{
		printf("%d\n", arr[j]);
	}
}

int _tmain(int argc, char* argv[])
{
	func();
	return 0;
}
#include "stdafx.h"

void func()
{
	int i;
	int arr[5] = {1, 2, 3, 4, 5};
	for (i = 0; i <= 5; i++)
	{
		arr[i] = 0;
		printf("arr[%d]µÄÖµÊÇ: %d\n", i, arr[i]);
	}

}

int main(int argc, char* argv[])
{
	func();
	return 0;
}


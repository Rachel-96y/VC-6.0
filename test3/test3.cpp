#include "stdafx.h"

int arr[10] = {2,7,5,9,22,4,8,22,3,18};

int g_r; 

void ArrSum()
{
	int sum = 0;
	for (int i = 0; i < 10; i++)
	{
		sum += arr[i];
	}
	g_r = sum;
}

int main(int argc, char* argv[])
{
	ArrSum();
	return 0;
}


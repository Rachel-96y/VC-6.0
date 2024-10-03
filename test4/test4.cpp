#include "stdafx.h"

int arr[10] = {2,7,5,9,22,4,8,22,3,18};

int g_r; 

void ArrMax()
{
	int temp = 0;
	for (int i = 0; i < 9; i++)
	{
		if (arr[i] > arr[i + 1])
		{
			temp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = temp;
		}
	}
	g_r = arr[9];
}

int main(int argc, char* argv[])
{
	ArrMax();
	return 0;
}


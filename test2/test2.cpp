#include "stdafx.h"

int arr[4] = {18,22,17,9};

int g_r; 

void ArrMax()

{
	int temp = 0;
	for (int i = 0; i < 3; i++)
	{
		if (arr[i] >= arr[i + 1])
		{
			temp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = temp;
		}
	}

	g_r = arr[3];
}

int main(int argc, char* argv[])
{
	ArrMax();
	return 0;
}
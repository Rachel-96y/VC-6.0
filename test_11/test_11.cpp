#include "stdafx.h"

int function(int x, int y, int z)
{
	int a = 0;
	int b = 1;
	int c = 2;
	if (x <= y)
	{
		a = b - 1;
	}
	else if(y >= z)
	{
		a = c + 1;
	}
	else if(x > z)
	{
		a = b + c;
	}
	else
	{
		a = b + c - 1;
	}
	return a + 1;
}

int main(int argc, char* argv[])
{
	function(4, 3, 5);
	return 0;
}
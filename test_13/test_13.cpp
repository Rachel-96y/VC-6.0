#include "stdafx.h"

int g_n = 100;

void function(int x, int y)
{
	int i = g_n;
	if (x > y)
	{
		g_n = x + i;
	}
	else
	{
		g_n = y + i;
	}
}

int main(int argc, char* argv[])
{
	function(5, 6);
	return 0;
}
#include "stdafx.h"

int g_n = 100; 

void function(int x, int y)
{
	int i = g_n;
	int j = 2;
	if (x >= y)
	{
		j += 1;
	}
	if (x < y)
	{
		g_n = j;		
	}
	else
	{
		g_n = i + j;
	}
	
}
	


int main(int argc, char* argv[])
{
	function(5, 6);
	return 0;
}
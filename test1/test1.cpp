#include "stdafx.h"

int g_x = 5;
int g_y = 8;
int g_z = 8;
int g_r = 0;

void Max()
{
	if (g_x > g_y)
	{
		g_r = g_x;
	}
	else
	{
		g_r = g_y;
	}
	if (g_r < g_z)
	{
		g_r = g_z;
	}
}

int main(int argc, char* argv[])
{
	Max();
	return 0;
}


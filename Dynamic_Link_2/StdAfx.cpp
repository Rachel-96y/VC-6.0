// stdafx.cpp : source file that includes just the standard includes
//	Dynamic_Link_2.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

int __stdcall Plus(int x,int y)			
{			
	return x+y;		
}			
int __stdcall Sub(int x,int y)			
{			
	return x-y;		
}			
int __stdcall Mul(int x,int y)			
{			
	return x*y;		
}			
int __stdcall Div(int x,int y)			
{			
	return x/y;		
}			

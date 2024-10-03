// B_CPP_7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
// 1、单继承无函数覆盖(打印Sub对象的虚函数表)

struct Base			
{			
public:			
    virtual void Function_1()			
    {			
        printf("Base:Function_1...\n");			
    }			
    virtual void Function_2()			
    {			
        printf("Base:Function_2...\n");			
    }			
    virtual void Function_3()			
    {			
        printf("Base:Function_3...\n");			
    }			
};		
	
struct Sub:Base			
{			
public:			
    virtual void Function_4()			
    {			
        printf("Sub:Function_4...\n");			
    }			
    virtual void Function_5()			
    {			
        printf("Sub:Function_5...\n");			
    }			
    virtual void Function_6()			
    {			
        printf("Sub:Function_6...\n");			
    }			
};			

*/



// 2、单继承有函数覆盖(打印Sub对象的虚函数表)				
				
struct Base				
{				
public:				
    virtual void Function_1()				
    {				
        printf("Base:Function_1...\n");				
    }				
    virtual void Function_2()				
    {				
        printf("Base:Function_2...\n");				
    }				
    virtual void Function_3()				
    {				
        printf("Base:Function_3...\n");				
    }				
};				
struct Sub:Base				
{				
public:				
    virtual void Function_1()				
    {				
        printf("Sub:Function_1...\n");				
    }				
    virtual void Function_2()				
    {				
        printf("Sub:Function_2...\n");				
    }				
    virtual void Function_6()				
    {				
        printf("Sub:Function_6...\n");				
    }				
};				



int main(int argc, char* argv[])
{
	/*
		本节练习：					
		1、单继承无函数覆盖(打印Sub对象的虚函数表)
		2、单继承有函数覆盖(打印Sub对象的虚函数表)				
	*/

	/*
	// 1、单继承无函数覆盖(打印Sub对象的虚函数表)
	Sub sub;
	typedef void(*Function)(void);
	Function pfunc_1;
	Function pfunc_2;
	Function pfunc_3;
	Function pfunc_4;
	Function pfunc_5;
	Function pfunc_6;

	LPDWORD* pSub = (LPDWORD*)&sub;

	pfunc_1 = (void (__cdecl *)(void))*(*(pSub + 0) + 0);
	pfunc_2 = (void (__cdecl *)(void))*(*(pSub + 0) + 1);
	pfunc_3 = (void (__cdecl *)(void))*(*(pSub + 0) + 2);
	pfunc_4 = (void (__cdecl *)(void))*(*(pSub + 0) + 3);
	pfunc_5 = (void (__cdecl *)(void))*(*(pSub + 0) + 4);
	pfunc_6 = (void (__cdecl *)(void))*(*(pSub + 0) + 5);

	pfunc_1();
	pfunc_2();
	pfunc_3();
	pfunc_4();
	pfunc_5();
	pfunc_6();

	// 结果: 
		// Base:Function_1...
		// Base:Function_2...
		// Base:Function_3...
		// Sub:Function_4...
		// Sub:Function_5...
		// Sub:Function_6...
	*/


	// 2、单继承有函数覆盖(打印Sub对象的虚函数表)
	
	Sub sub;
	typedef void(*Function)(void);
	Function pfunc_1;
	Function pfunc_2;
	Function pfunc_3;
	Function pfunc_4;

	LPDWORD* pSub = (LPDWORD*)&sub;

	pfunc_1 = (void (__cdecl *)(void))*(*(pSub + 0) + 0);
	pfunc_2 = (void (__cdecl *)(void))*(*(pSub + 0) + 1);
	pfunc_3 = (void (__cdecl *)(void))*(*(pSub + 0) + 2);
	pfunc_4 = (void (__cdecl *)(void))*(*(pSub + 0) + 3);

	pfunc_1();
	pfunc_2();
	pfunc_3();
	pfunc_4();

	// 结果:
		// Sub:Function_1...
		// Sub:Function_2...
		// Base:Function_3...
		// Sub:Function_6...

	getchar();
	return 0;
}


// B_CPP_6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class MyStruct
{

private:

	int a;
	int b;

	void function()
	{
		printf("我没有被放在虚函数表中\n");
	}
	virtual void function1()
	{
		printf("哈哈哈\n");
	}

	virtual void function2()
	{
		printf("啦啦啦\n");
	}

public:

	MyStruct()
	{
		a = 1;
		b = 2;
	}


};

int main(int argc, char* argv[])
{
	MyStruct MS;
	LPDWORD pMS = (LPDWORD)&MS;
	printf("%x\n%x\n%x\n", *(pMS + 0), *(pMS + 1), *(pMS + 2));
	void(*pFunc1)(void) = (void(*)(void))*((LPDWORD)*(pMS + 0) + 0);
	pFunc1();
	void(*pFunc2)(void) = (void(*)(void))*((LPDWORD)*(pMS + 0) + 1);
	pFunc2();
	getchar();

	return 0;
}


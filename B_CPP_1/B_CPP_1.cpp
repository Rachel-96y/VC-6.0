// B_CPP_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef struct MyStruct 
{
	// 两个 Int 类型的成员X，Y
	INT X;
	INT Y;
	
	// 分别实现对X Y的加法、减法、乘法与除法的功能.
	Plus()
	{
		return X + Y;
	}

	Sub()
	{
		return X - Y;
	}

	Mul()
	{
		return X * Y;
	}

	Div()
	{
		return X / Y;
	}

} MySt, *PMySt;

typedef struct StructVoid 
{
	
} St, *PSt; 


int main(int argc, char* argv[])				
{			
	/*
		本节练习：								
								
			1、设计一个结构体，有两个Int类型的成员X，Y 在结构体内部定义4个函数								
											
			分别实现对X Y的加法、减法、乘法与除法的功能.								
				
				  
			2、观察这些函数调用的时候，与其他的函数调用有哪些不同？								
											
			从参数传递、压栈顺序、堆栈平衡来总结.								
				
				  
			3、空结构体的大小是多少？为什么？								
				
				  
			4、下面代码能否执行？								
																	
			struct Person  								
			{  								
				int x ;							
				void Fn_1()								
				{							
					printf("Person:Fn_1()\n");						
				}							
				void Fn_2() 								
				{							
					x = 10;						
					printf("Person:Fn_2()%x\n");						
				}							
			};  								
  											
											
			int main(int argc, char* argv[])								
			{								
				Person* p = NULL;							
											
				p -> Fn_1();							
				p -> Fn_2();							
											
				return 0;							
			}								

	*/
	
	/*
		1、设计一个结构体，有两个Int类型的成员X，Y 在结构体内部定义4个函数																
		分别实现对X Y的加法、减法、乘法与除法的功能.
	*/

	MySt mySt;
	mySt.X = 10;
	mySt.Y = 5;
	INT result1 = mySt.Plus();
	INT result2 = mySt.Sub();
	INT result3 = mySt.Mul();
	INT result4 = mySt.Div();

	// answer = 15 ,5 ,50 ,2
	printf("%d, %d, %d, %d \n", result1, result2, result3, result4);

	
	

	/*
		2、观察这些函数调用的时候，与其他的函数调用有哪些不同？								
		从参数传递、压栈顺序、堆栈平衡来总结.

		这些函数在调用前都会通过 ECX 传递一个 this 指针(结构体的首地址)到函数中
		压栈顺序是先从右至左压函数需要的参数再压入 this 指针
		函数的堆栈平衡从外平栈变成了内平栈

	*/

	/*
		3、空结构体的大小是多少？为什么？

		空结构体的大小是 1 字节 
		C++标准中规定 任何不同的对象不能拥有相同的内存地址如果空类大小为0，
		我们声明一个这个类的对象数组，那么数组中的每个对象都拥有了相同的地址

		空结构体（不含数据成员）的sizeof值为1。试想一个“不占空间“的变量如何被取地址、
		两个不同的“空结构体”变量又如何得以区分呢，于是，“空结构体”变量也得被存储，
		这样编译器也就只能为其分配一个字节的空间用于占位了


	*/
	printf("%d\n", sizeof(St));
	
	
	/*	
  		4、下面代码能否执行？								
											
											
			struct Person  								
			{  								
				int x ;							
				void Fn_1()								
				{							
					printf("Person:Fn_1()\n");						
				}							
				void Fn_2() 								
				{							
					x = 10;						
					printf("Person:Fn_2()%x\n");						
				}							
			};  								
  											
											
			int main(int argc, char* argv[])								
			{								
				Person* p = NULL;							
											
				p -> Fn_1();							
				p -> Fn_2();							
											
				return 0;							
			}	

		不能执行 因为没有使用这个结构体声明结构体变量
		而是声明了这个结构体的指针 但是这个结构体的指针
		却指向了 NULL 这将引发 C00000005 错误
	*/

	getchar();

	return 0;			
}



								
  								
								

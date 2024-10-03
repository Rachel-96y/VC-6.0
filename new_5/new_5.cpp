#include "stdafx.h"

/*
	练习：								
								
								
1、定义一个结构体Gamer用来存储一个游戏中的角色的信息，包括血值、等级、坐标等信息								
								
要求：								
								
	(1) 具体包含哪些信息自由设计							
					
																
	(2) 但这些包含的类型中，必须要有一个成员是结构体类型		
								
2、定义一个函数，用来给这个结构体变量赋值.								
								
3、定义一个函数，用来显示这个结构体变量的所有成员信息.								
								

*/

struct Point
{
	// 坐标的结构体
	double x;
	double y;
	double z;
};

struct Gamer
{
	// 游戏角色结构体
	int blood;
	int part;
	Point location;
};


Gamer charactor;  // 声明Gamer类型的全局变量


void input()
{
	// 给结构体变量赋值
	charactor.blood = 100;
	charactor.part = 50;
	charactor.location.x = 25.3;
	charactor.location.y = 30.2;
	charactor.location.z = 10.9;
}


void output()
{
	// 显示结构体变量的所有成员信息
	printf("血量: %d\n", charactor.blood);
	printf("等级: %d\n", charactor.part);
	printf("x - y - z 坐标: %.1f - %.1f - %.1f\n", \
		charactor.location.x, \
		charactor.location.y, \
		charactor.location.z);
}


int main()
{
	input();
	output();
	getchar();
	return 0;
}

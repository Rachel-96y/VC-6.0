// B_CPP_10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class Person				
{
private:		
	int x;			
	int y;

public:	
	Person(int x,int y)			
	{			
		this -> x = x;		
		this -> y = y;		
	}		

	Person operator = (const Person& p);

	friend Person operator ++ (Person& p);
	friend Person operator -- (Person& p);
	friend Person operator + (Person* p, const Person& q);
	friend Person operator - (Person* p, const Person& q);
	friend Person operator * (Person* p, const Person& q);
	friend Person operator / (Person* p, const Person& q);
	friend bool operator > (Person* p, const Person& q);
	friend bool operator < (Person* p, const Person& q);
	friend bool operator >= (Person* p, const Person& q);
	friend bool operator <= (Person* p, const Person& q);
	friend bool operator == (Person* p, const Person& q);

};

Person Person::operator = (const Person& p)
{
	this -> x = p.x;
	this -> y = p.y;

	return p;
}
			
Person operator ++ (Person& p)
{
	p.x++;
	p.y++;

	return p;
}

Person operator -- (Person& p)
{
	p.x--;
	p.y--;

	return p;
}

Person operator + (Person* p, const Person& q)
{
	p -> x = p -> x + q.x;
	p -> y = p -> y + q.y;

	return *p;
}

Person operator - (Person* p, const Person& q)
{
	p -> x = p -> x - q.x;
	p -> y = p -> y - q.y;

	return *p;
}

Person operator * (Person* p, const Person& q)
{
	p -> x = p -> x * q.x;
	p -> y = p -> y * q.y;

	return *p;
}

Person operator / (Person* p, const Person& q)
{
	p -> x = p -> x / q.x;
	p -> y = p -> y / q.y;

	return *p;
}

bool operator > (Person* p, const Person& q)
{
	if ((p -> x > q.x) && (p -> y > q.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool operator < (Person* p, const Person& q)
{
	if ((p -> x < q.x) && (p -> y < q.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool operator >= (Person* p, const Person& q)
{
	if ((p -> x >= q.x) && (p -> y >= q.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool operator <= (Person* p, const Person& q)
{
	if ((p -> x <= q.x) && (p -> y <= q.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool operator == (Person* p, const Person& q)
{
	if ((p -> x == q.x) && (p -> y == q.y))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int main(int argc, char* argv[])
{
	/*
		本节练习：
			1、定义一个类，使用友元函数实现：+、-、*、/、>、<、>=、<=、 ==、 =
			等运算符重载(什么情况下，运算符重载一定要用友元函数?).
			2、从反汇编的角度说说引用与指针的区别.
	*/

	// 引用与指针没有区别 引用是编译器限制

	getchar();
	
	return 0;
}
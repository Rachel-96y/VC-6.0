// B_CPP_8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class Base				
{				
public:				
	int x;			
public:				
	Base()			
	{			
		x = 100;		
	}			
    void Function_1()				
    {				
        printf("Base:Function_1...\n");				
    }				
    virtual void Function_2()				
    {				
        printf("Base:Function_2...virtual\n");				
    }				
};
				
class Sub:public Base				
{				
public:				
	int x;			
public:				
	Sub()			
	{			
		x = 200;		
	}			
    void Function_1()				
    {				
        printf("Sub:Function_1...\n");				
    }				
    virtual void Function_2()				
    {				
        printf("Sub:Function_2...virtual\n");				
    }				
};				

void TestBound(Base* pb)				
{				
	int n = pb->x;	
	
	printf("%x\n",n);			
				
	pb->Function_1();		//函数调用	
				
	pb->Function_2();			
}				


int main(int argc, char* argv[])
{
	/*
		本节练习：	
	
			1、定义一个父类：Base 有两个成员X,Y 有一个函数 Print (非 virtul )	
			能够打印X,Y的值.	
				
			定义3个子类：Sub1  有一个成员A	
				
					Sub2  有一个成员B
				
					Sub3  有一个成员C
				
					
			每个子类有一个函数 Print (非 virtul )，打印所有成员.	
				
			Sub1:打印X Y A	
				
			Sub2:打印X Y B	
				
			Sub3:打印X Y C	
				
			2、定义一个数组，存储 Base Sub1 Sub2 Sub3 使用一个循环语句	
			调用所有的 Print 函数。	
				
			3、将上面所有的 Print 函数改成 virtul 继续观察效果.	
				
			4、思考题：为什么析构函数建议写成 virtul 的？	
				
			5、写一个例子程序，能体现多态的作用.	

				#include <iostream>
				#include <cmath>
				using namespace std;
				class CShape  //基类：形体类
				{
					public:
						virtual double Area() { };  //求面积
						virtual void PrintInfo() { }; //显示信息
				};
				class CRectangle:public CShape  //派生类：矩形类
				{
					public:
						int w,h;     //宽和高
						virtual double Area();
						virtual void PrintInfo();
				};
				class CCircle:public CShape  //派生类：圆类
				{
					public:
						int r;      //半径
						virtual double Area();
						virtual void PrintInfo();
				};
				class CTriangle:public CShape //派生类：三角形类
				{
					public:
						int a,b,c;      //三边长
						virtual double Area();
						virtual void PrintInfo();
				};
				double CRectangle::Area()  {
					return w * h;
				}
				void CRectangle::PrintInfo()  {
					cout << "Rectangle:" << Area() << endl;
				}
				double CCircle::Area()  {
					return 3.14 * r * r ;
				}
				void CCircle::PrintInfo()  {
					cout << "Circle:" << Area() << endl;
				}
				double CTriangle::Area()  {   //根据海伦公式计算三角形面积
					double p = ( a + b + c) / 2.0;
					return sqrt(p * ( p - a)*(p- b)*(p - c));
				}
				void CTriangle::PrintInfo()  {
					cout << "Triangle:" << Area() << endl;
				}
				CShape *pShapes[100]; //用来存放各种几何形体，假设不超过100个
				int MyCompare(const void *s1, const void *s2)  //定义排序规则的函数
				{
					CShape **p1 = (CShape **)s1; //s1是指向指针的指针,其指向的指针为CShape* 类型
					CShape **p2 = ( CShape **)s2;
					double a1 = (*p1)->Area(); //p1指向几何形体对象的指针, *p1才指向几何形体对象
					double a2 = (*p2)->Area();
					if( a1 < a2 )
						return -1;   //面积小的排前面
					else if (a2 < a1)
						return 1;
					else
						return 0;
				}
				int main()
				{
					int i; int n;
					CRectangle *pr; CCircle *pc; CTriangle *pt;
					cin >> n;
					for( i = 0;i < n;++i ) {
						char c;
						cin >> c;
						switch(c) {
							case 'R': //矩形
							pr = new CRectangle();
							cin >> pr->w >> pr->h;
							pShapes[i] = pr;
							break;
							 case 'C': //圆
							pc  = new CCircle();
							cin >> pc->r;
							pShapes[i] = pc;
							break;
							case 'T': //三角形
							pt = new CTriangle();
							cin >> pt->a >> pt->b >> pt->c;
							pShapes[i] = pt;
							break;
						}
					}
					qsort(pShapes,n,sizeof(Cshape *),MyCompare);
					for(i = 0;i <n;++i) {
						pShapes[i]->PrintInfo();
						delete pShapes[i]; //释放空间
					}
					return 0;
				}

	*/
	Base a;
	Sub b;
	Base* arr[2] = {&a, &b};
	for (int i = 0; i < 2; i++)
	{
		TestBound(arr[i]);
	}
	getchar();
	return 0;
}

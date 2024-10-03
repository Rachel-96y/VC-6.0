// B_CPP_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


typedef struct DateInfo
{
	int year; 
	int month;
	int day;

	DateInfo(int year, int month, int day)
	{
		this -> year = year;
		this -> month = month;
		this -> day = day;
	}

	DateInfo()
	{
		year = 2015;
		month = 4;
		day = 2;
	}

	void SetDay(int day)
	{
		this -> day = day;
		return;
	}

	int GetDay()
	{
		return day;
	}

	void SetMonth(int month)
	{
		this -> month = month;
		return;
	}

	int GetMonth()
	{
		return month;
	}
	
	void SetYear(int year)
	{
		this -> year = year;
		return;
	}
	
	int GetYear()
	{
		return year;
	}
	
} DI, *PDI;


typedef struct TimeInfo : DateInfo
{
	int Hour;
	int minute;
	int second;

	TimeInfo(int time, int minute, int second)
	{
		this -> Hour = Hour;
		this -> minute = minute;
		this -> second = second;
	}

	TimeInfo()
	{
		Hour = 23;
		minute = 13;
		second = 45;
	}

	int GetHour()
	{
		return Hour;
	}
	
	int GetMinute()
	{
		return minute;
	}

	int GetSecond()
	{
		return second;
	}

} TI, *PTI; 


typedef struct MyString
{
	LPVOID pFileBuffer;
	BYTE String[100];

	MyString(UINT SizeOfFileBuffer)
	{
		pFileBuffer = malloc(sizeof(CHAR) * SizeOfFileBuffer);
	}

	MyString()
	{
		pFileBuffer = malloc(sizeof(CHAR) * 1024);
	}

	~MyString()
	{
		free(pFileBuffer);
	}

	void SetString(LPSTR String)
	{
		strcpy((LPSTR)this -> String, String);
		return;
	}

	void PrintString()
	{
		printf("String: %s\n", String);
		return;
	}

	void AppendString(LPSTR String)
	{
		LPSTR pTemp = (LPSTR)this -> String;
		while(*pTemp++);
		strcpy(--pTemp, String);
		return;
	}
	
	int Size()
	{
		return sizeof(String);
	}

} MS, *PMS;


void Run()
{
	TI MyTimeInfo;
	PTI pMyTimeInfo = &MyTimeInfo;
	pMyTimeInfo -> year;
	pMyTimeInfo -> month;
	pMyTimeInfo -> day;
	pMyTimeInfo -> Hour;
	pMyTimeInfo -> minute;
	pMyTimeInfo -> second;

	PDI pMyDateInfo = &MyTimeInfo;
	pMyDateInfo -> year;
	pMyDateInfo -> month;
	pMyDateInfo -> day;

	return;
}


LPVOID Test(int whoami)
{
	MS MyStr_1(200);

	*(LPDWORD)MyStr_1.pFileBuffer = 0x12345678;
	MyStr_1.SetString("MyCode_1");
	MyStr_1.PrintString();
	MyStr_1.AppendString("HelloWorld");
	MyStr_1.PrintString();
	int result_1 = MyStr_1.Size();
	printf("%d\n", result_1);

	MS MyStr_2;

	*(LPDWORD)MyStr_2.pFileBuffer = 0x12345678;
	MyStr_2.SetString("MyCode_2");
	MyStr_2.PrintString();
	MyStr_2.AppendString("HelloWorld");
	MyStr_2.PrintString();
	int result_2 = MyStr_2.Size();
	printf("%d\n", result_2);

	if (!whoami)
	{
		return MyStr_1.pFileBuffer;
	}
	else
	{
		return MyStr_2.pFileBuffer;
	}	
}

int main(int argc, char* argv[])
{
	/*
		本节练习：							
							
			1	设计一个结构 DateInfo 要求其满足下述要求。							
   										
				(1) 有三个成员： int year; int month; int day;							
				(2) 要求有个带参数的构造函数，其参数分别为对应年、月、日。							
				(3) 有一个无参数的构造函数，其初始的年、月、日分别为：2015、4、2。							
				(4) 要求有一个成员函数实现日期的设置: SetDay(int day)							
				(5) 要求有一个成员函数实现日期的获取: GetDay()							
				(6) 要求有一个成员函数实现年份的设置: SetYear(int year)							
				(7) 要求有一个成员函数实现年份的获取: GetYear()							
				(8) 要求有一个成员函数实现月份的设置: SetMonth(int month)							
				(9) 要求有一个成员函数实现月份的获取: GetMonth()							
										
										
			2	设计一个结构 TimeInfo 要求其满足下述要求。							
				(1) 该结构中包含表示时间的时、分、秒。							
				(2) 设置该结构中时、分、秒的函数。							
				(3) 获取该结构中时、分、秒的三个函数： GetHour(), GetMinute() 和 GetSecond()							
										
										
			3	让 TimeInfo 继承 DateInfo 分别使用 DataInfo 和 TimeInfo 的指针访问 TimeInfo							
				对象的成员.							
										
			4	设计一个结构叫做 MyString ，要求该结构能够完成以下功能：							
				(1) 构造函数能够根据实际传入的参数分配实际存储空间；							
				(2) 提供一个无参的构造函数，默认分配大小为 1024 个字节；							
				(3) 析构函数释放该空间；							
				(4) 编写成员函数 SetString ，可以将一个字符串赋值给该结构；							
				(5) 编写成员函数 PrintString ，可以将该结构的内容打印到屏幕上；							
				(6) 编写成员函数 AppendString ，用于向已有的数据后面添加数据；							
				(7) 编写成员函数 Size ，用于得到当前数据的真实长度。							
				编写测试程序，测试这个结构。							

	*/
	
	Run();

	LPVOID pFileBuffer = Test(0);
	if (*(LPDWORD)pFileBuffer == 0xDDDDDDDD)
	{
		printf("返回了 MyStr_1.pFileBuffer / MyStr_1的析构函数执行成功!\n*(LPDWORD)pFileBuffer: %x\n", *(LPDWORD)pFileBuffer);
	}
	else
	{
		printf("返回了 MyStr_2.pFileBuffer /  \n *(LPDWORD)pFileBuffer的值是: %x\n", *(LPDWORD)pFileBuffer);
	}

	getchar();


	return 0;
}


// stdafx.cpp : source file that includes just the standard includes
//	project_1.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

// ------以下为函数实现------


// ************************************************************************

DateInfo::DateInfo(int year, int month, int day)
{
	this -> year = year;
	this -> month = month;
	this -> day = day;
}

DateInfo::DateInfo()
{
	year = 2015;
	month = 4;
	day = 2;
}

void DateInfo::SetDay(int day)
{
	this -> day = day;
	return;
}

int DateInfo::GetDay()
{
	return day;
}

void DateInfo::SetMonth(int month)
{
	this -> month = month;
	return;
}

int DateInfo::GetMonth()
{
	return month;
}

void DateInfo::SetYear(int year)
{
	this -> year = year;
	return;
}

int DateInfo::GetYear()
{
	return year;
}

// ************************************************************************

	TimeInfo::TimeInfo(int time, int minute, int second)
	{
		this -> Hour = Hour;
		this -> minute = minute;
		this -> second = second;
	}

	TimeInfo::TimeInfo()
	{
		Hour = 23;
		minute = 13;
		second = 45;
	}

	int TimeInfo::GetHour()
	{
		return Hour;
	}
	
	int TimeInfo::GetMinute()
	{
		return minute;
	}

	int TimeInfo::GetSecond()
	{
		return second;
	}

// ****************************************************************************

MyString::MyString(UINT SizeOfFileBuffer)
{
	pFileBuffer = malloc(sizeof(CHAR) * SizeOfFileBuffer);
}

MyString::MyString()
{
	pFileBuffer = malloc(sizeof(CHAR) * 1024);
}

MyString::~MyString()
{
	free(pFileBuffer);
}

void MyString::TestFileBuffer()
{
	*(LPDWORD)pFileBuffer = 0x12345678;
}

LPVOID MyString::RETN_VALUE()
{
	return pFileBuffer;
}

void MyString::SetString(LPSTR String)
{
	strcpy((LPSTR)this -> String, String);
	return;
}

void MyString::PrintString()
{
	printf("String: %s\n", String);
	return;
}

void MyString::AppendString(LPSTR String)
{
	LPSTR pTemp = (LPSTR)this -> String;
	while(*pTemp++);
	strcpy(--pTemp, String);
	return;
}

int MyString::Size()
{
	return sizeof(String);
}

// ****************************************************************************
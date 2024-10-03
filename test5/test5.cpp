#include "stdafx.h"

// 此问题暂时未解决

void StrFunction(int n)
{
	char* g_n = "china中国verygood天朝nice";
	/* 如果是字符串的话，比如上面这个字符串
	放在堆栈缓冲区里 ebp -4 位置的是此字符串所在的地址，
	即：g_n中前4个字节为一个有效地址。
	而不会直接是字符串的值。*/	
}

int main(int argc, char* argv[])
{
	StrFunction(6);  // 截取出来的结果应该是: china中
	return 0;
}

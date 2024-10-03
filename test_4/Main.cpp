#include <stdio.h>

#pragma pack(1)   //这里设定变量以N字节进行对齐
struct test_1{
	char x1;
	int x2;
};
#pragma pack()

void main()
{
	test_1 a = {1,2};
	printf("%d\n", sizeof(a));  //默认情况下字节对齐后使用的空间是:8字节   
	return;      //结构体总大小是N的倍数   N = Min(最大成员,对齐参数)
}


//那我也可以算是黑客: (在gcc或者vc6下编译)(32位)

#include <stdio.h>

void func_2(){
	printf("加油啊!\n");
	return;
}

void func_1(){
	int arr[10];
	arr[11] = (int)&func_2;   //覆盖ebp+4的值
	return;
}

void main()
{
	func_1();
	return;
}
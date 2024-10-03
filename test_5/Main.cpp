//极致内存节省版(运行速度最慢版)
#include <stdio.h>
#include <Windows.h>

int* A;
A = (int*)3;


#pragma pack(1)  //这里的参数只能是1/2/4/8中的 
struct stA_ {
	short a;
	char b;
};
#pragma pack()

void main() 
{
	struct stA_ arry[5] = {{1, 2}, {3, 5}, {4, 6}, {0, 7}, {8, 9}};  //8个字节
	for(int i = 0; i < 5; i++)
		printf("%d\t%d\n", arry[i].a,arry[i].b);
	system("pause");
	return;
}
#include <stdio.h>
#include <Windows.h>
void main()  //入口程序   程序开始执行的地方
{
	//判断输入的值是否是正确的
	char arry[6] = {'1', '5', '6', '7', '4', '9'};
	char c = 0;
	char i = 0;
	printf("请输入密码:\n");
	while(1){
		c = getchar();
		if(c == '\n' && i == 6){
			printf("密码正确!");
			Sleep(2000);
			return;
		}
		if (c != arry[i]){
			printf("密码错误!");
			Sleep(2000);
			return;
		}
		i++;
	}
}
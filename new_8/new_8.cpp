#include "stdafx.h"


void Function()					
{					
	int arr[5] = {1,2,3,4,5};				
					
	//..此处添加代码，使用指针，将数组的值倒置
	int temp;
	int* p = &arr[0];
	for(int i = 0, j = 4; i != j; i++, j--)
	{
		temp = *(p + i);
		*(p + i) = *(p + j);
		*(p + j) = temp;
	}
					
	//打印数组值的代码已经写完，不需要修改				
	for(int k = 0; k < 5; k++)				
	{				
		printf("%d\n",*(p+k));			
	}				
}

int main()
{ 
	Function();
	return 0;
}
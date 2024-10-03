#include <stdio.h>
#include <stdlib.h>
int arry[3] = {1, 2, 3};
void where();
void main()
{
	where();
	int answer = 0;

	__asm{
		MOV dword ptr ds:[eax+12],10
	}

	answer = arry[3];
	printf("%d\n", answer);
	system("pause");
	return;
}

void where(){
	int* a = NULL;
	a = &arry[0];
	__asm{
		MOV eax, a
	}
	return;
}
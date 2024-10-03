#include "stdafx.h"

int __declspec(naked) __stdcall Func_1(int x)
{
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
		SUB ESP, 44H
		PUSH EBX
		PUSH ESI 
		PUSH EDI
		MOV EAX, 0CCCCCCCCH
		MOV ECX, 11H
		LEA EDI, DWORD PTR DS:[EBP-44H]
		REP STOS DWORD PTR [EDI]

	}

	x++;

	__asm
	{
		MOV EAX, DWORD PTR DS:[EBP+8]
		MOV DWORD PTR DS:[EBP-4], EAX
		MOV EAX, DWORD PTR DS:[EBP-4]
		ADD EAX, 1
		POP EDI
		POP ESI
		POP EBX
		MOV ESP, EBP
		POP EBP
		RETN 4
	}
}

int main(int argc, char* argv[])
{
	int x = Func_1(5);
	printf("%d\n", x);
	return 0;
}
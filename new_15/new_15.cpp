/*
今天练习：								
								
1、定义一个 unsigned char 类型，通过程序为第3、5、7位赋值,赋值时不能								
影响到其它位原来的值.								
(使用位操作指令、比如：& | ~ ^ << >>等)								
								
2、判断某个位的值是否为1.								
(使用位操作指令、比如：& | ~ ^ << >>等)								
								
3、读取第7、6、5位的值，以十进制显示(unsigned).								
(使用位操作指令、比如：& | ~ ^ << >>等)								
								
4、用十六进制文本编辑器分别打开一个.exe、.dll、.sys、.txt、.doc								
.jpg、.pdf等将前四个字节写在下面.								
								
5、将一个在十六进制编辑器中打开的.exe文件，拖拽到最后，
观察文件中的大小和硬盘上的大小.	
							
(下载一个WinHex)								
*/

#include "stdafx.h"


void function()
{	
	// 1、定义一个 unsigned char 类型，通过程序为第3、5、7位赋值
	// 赋值时不能影响到其它位原来的值.

	unsigned char num;
	num = 0x01;		// 0000 0001  参数
	unsigned char Flag;
	Flag = 0x04;	// 0000 0100
	num |= Flag;
	printf("%x\n", num);

	Flag = 0x10;	// 0001 0000
	num |= Flag;
	printf("%x\n", num);

	Flag = 0x40;	// 0100 0000
	num |= Flag;  
	printf("%x\n", num);

	Flag = 0x40;
	if (num & Flag)
		num ^= Flag;
	printf("%x\n", num);

	Flag = 0x10;
	if (num & Flag)
		num ^= Flag;
	printf("%x\n", num);

	Flag = 0x04;
	if (num & Flag)
		num ^= Flag;
	printf("%x\n", num);



	// 2、判断某个位的值是否为1.   判断第4位
	Flag = 0x08;   // 0000 1000
	if (Flag & num)
		printf("第4位的值为1\n");
	else
		printf("第4位的值为0\n");



	// 3、读取第7、6、5位的值，以十进制显示(unsigned).
	num = 0xEA;   // 1110 1010
	Flag = 0x70;  // 0111 0000 
	num &= Flag;
	num >>= 4;   // 有符号数右移高位补符号位,无符号数补0
	printf("%d\n", num);

}

// 4、用十六进制文本编辑器分别打开一个.exe、.dll、.sys、.txt、.doc、
// .jpg、.pdf等将前四个字节写在下面.	

// (hex)
// .exe  4D 5A 90 00
// .dll  4D 5A 90 00
// .sys  4D 5A 90 00
// .txt  43 6F 70 79  /  43 61 74 61  / 20 20 4C 49 (为写入的文本)
// .doc  D0 CF 11 E0
// .docx 50 4B 03 04
// .jpg  FF D8 FF E0
// .pdf  25 50 44 46
// .ini  FF FE 0D 00


// 5、将一个在十六进制编辑器中打开的.exe文件，拖拽到最后，
// 观察文件中的大小和硬盘上的大小.	

// 文件大小(0xBCB2)： 47.1 KB (48,306 字节)  占用空间(硬盘上的大小)：48.0 KB (49,152 字节)
// 在文件中的大小比占用空间更小


int main(int argc, char* argv[])
{

	function();
	getchar();
	return 0;
}
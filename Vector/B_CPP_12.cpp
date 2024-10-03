// B_CPP_12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template <class T_Dynamic> class Vector  // 一个容器
{
private:	// 私有变量
	DWORD			e_dwMagic;			// 当前容器的成员个数
	DWORD			e_dwNextIndex;		// 下一个索引
	DWORD			e_dwLfanew;			// 默认的创建对象的个数
	DWORD			e_dwLength;			// 当前容器的长度
	DWORD			e_dwIncrement;		// 每次增容的大小
	T_Dynamic*		e_pAddress;			// 容器首地址指针

private:	// 私有函数
	BOOL WINAPI expand(void);			// 当容器大小不够时进行扩充

public:		// 构造函数及析构函数
	Vector();
	Vector(IN DWORD dwNumberOfTypes);
	~Vector();

public:		// 公有函数
	T_Dynamic*		WINAPI append(void);														// 在末尾追加一个元素
	T_Dynamic*		WINAPI append(IN DWORD dwAppendNumber);										// 在末尾追加多个元素(限制了最多 5 个)
	T_Dynamic*		WINAPI insert(IN DWORD dwIndexNumber);										// 根据下标插入元素 
	T_Dynamic*		WINAPI insert(IN DWORD dwIndexNumber, IN T_Dynamic pT_Dynamic);				// 根据下标插入指定的元素
	T_Dynamic*		WINAPI pointerToIndex(IN DWORD dwIndexNumber);								// 根据所给下标返回一个此元素的指针
	T_Dynamic*		WINAPI pointerToIndex(IN DWORD dwIndexNumber,OUT T_Dynamic pT_Dynamic);		// 根据给定的索引得到元素 并将值复制到 Vector 的其它位置
	T_Dynamic*		WINAPI push(IN T_Dynamic pT_Dynamic);										// 在末尾插入指定元素
	VOID			WINAPI pop(void);															// 删除最后一个元素
	DWORD			WINAPI capacity(void);														// 返回还能存储多少元素
	VOID			WINAPI clear(void);															// 清空所有元素	
	BOOL			WINAPI IsEmpty(void);														// 判断 Vector 是否为空 返回 TURE 时为空	
	VOID			WINAPI erase(IN DWORD dwIndexNumber);										// 根据下标删除指定元素
	DWORD			WINAPI size(void);															// 返回 Vector 全部元素数量的大小
	VOID			WINAPI ReadVectorMemberInfo(const Vector& pVector);							// 读取 Vector 中的变量的值

public:		// 重载 >、 >= 、 < 、 <=、 ==
	BOOL			WINAPI operator > (const Vector& pVector);		// 重载 ">"  :	如果 A -> e_dwLength >  B.e_dwLength	则A 大于 B
	BOOL			WINAPI operator >= (const Vector& pVector);		// 重载 ">=" :	如果 A -> e_dwLength >= B.e_dwLength	则A 大于 B
	BOOL			WINAPI operator < (const Vector& pVector);		// 重载 "<"  :	如果 A -> e_dwLength <  B.e_dwLength	则A 大于 B
	BOOL			WINAPI operator <= (const Vector& pVector);		// 重载 "<=" :	如果 A -> e_dwLength <= B.e_dwLength	则A 大于 B
	BOOL			WINAPI operator == (const Vector& pVector);		// 重载 "==" :	如果 A -> e_dwLength == B.e_dwLength	则A 大于 B
};

template <class T_Dynamic> BOOL WINAPI Vector<T_Dynamic>::expand(void)
{
	/* 
		如果容器需要扩容则执行此函数
		此函数每次扩容的大小为 e_dwIncrement
		此函数会将旧的数据复制到新申请的扩容后的地址
		函数执行成功则返回0 否则返回-1
	*/


	// 重新申请一块空间 (原长度 + 0xA)
	T_Dynamic* New_e_pAddress = new T_Dynamic[this -> e_dwMagic + this -> e_dwIncrement];

	if (!New_e_pAddress)
	{
		printf("在 expand 空间申请失败");
		return 0xFFFFFFFF;
	}

	// 初始化空间
	memset(New_e_pAddress, 0x00000000, (this -> e_dwMagic + this -> e_dwIncrement) * sizeof(T_Dynamic));

	// 将数据复制到新的空间
	memcpy(New_e_pAddress, this -> e_pAddress, sizeof(CHAR) * this -> e_dwLength);

	// 释放旧的空间
	delete[] this -> e_pAddress;

	// 更新成员的数据
	this -> e_dwMagic += this -> e_dwIncrement;
	this -> e_dwLength = sizeof(T_Dynamic) * this -> e_dwMagic;
	this -> e_pAddress = New_e_pAddress;
	
	return NULL;
}

template <class T_Dynamic> Vector<T_Dynamic>::Vector()
: e_dwLfanew(0x0000000A), e_dwMagic(0x0000000A), e_dwNextIndex(0x00000001), e_dwIncrement(0x000000A)
{
	// 给结构体中的变量赋值						
	this -> e_dwLength = sizeof(T_Dynamic) * this -> e_dwMagic;						

	//如果使用此函数进行初始化 则使用默认的 e_lfanew = 10 进行创建
	this -> e_pAddress = new T_Dynamic[this -> e_dwLfanew];	
	
	if (!this -> e_pAddress)
	{
		// 申请空间失败
		printf("在构造函数 Vector 中内存申请失败\n");
	}

	// 初始化空间
	memset(this -> e_pAddress, 0x00000000, sizeof(T_Dynamic) * this -> e_dwLfanew);
}

template <class T_Dynamic> Vector<T_Dynamic>::Vector(IN DWORD dwNumberOfTypes)
: e_dwLfanew(0x0000000A), e_dwIncrement(0x0000000A)
{
	// 给结构体中的变量赋值							
	this -> e_dwMagic = dwNumberOfTypes;								
	this -> e_dwNextIndex = this -> e_dwMagic;					
	this -> e_dwLength = sizeof(T_Dynamic) * this -> e_dwMagic;	
						
	// 根据给定的 NumberOfTypes 的值及 T_Dynamic 类型来申请空间
	this -> e_pAddress = new T_Dynamic[this -> e_dwMagic];	

	// 初始化空间
	memset(this -> e_pAddress, 0x00000000, sizeof(T_Dynamic) * this -> e_dwMagic);
}

template <class T_Dynamic> Vector<T_Dynamic>::~Vector()
{
	// 释放空间
	delete[] this -> e_pAddress;
	this -> e_pAddress = NULL;
}

template <class T_Dynamic> T_Dynamic* WINAPI Vector<T_Dynamic>::append(void)
{
	/* 
		如果 append 函数没有参数则向后追加一个单位的长度 
		返回值为新增的元素的指针 
	*/
	
	// 判断是否空间足够
	if (this -> e_dwNextIndex >= this -> e_dwMagic)
	{
		// 如果当前可用索引大于等于了当前容器的成员个数则表示需要扩容
		this -> expand();
	}

	// 更新成员变量
	this -> e_dwNextIndex++;		// 下一个可用索引

	// 返回此成员的指针
	T_Dynamic* pAppendElement = this -> e_pAddress + this -> e_dwNextIndex - 1;

	return pAppendElement;
}

template <class T_Dynamic> T_Dynamic* WINAPI Vector<T_Dynamic>::append(IN DWORD dwAppendNumber)
{
	/* 
		如果 append 函数有参数则向后追加 appendNumber 个单位的长度 
		返回值为新增的元素的指针 
	*/

	// 对 dwAppendNumber 进行限制
	if ((INT)dwAppendNumber < 1 || (INT)dwAppendNumber > 5)
	{
		printf("元素添加失败! 输入的值不能小于1或大于5\n");
		return NULL;
	}

	// 判断是否空间足够
	if ((this -> e_dwNextIndex + dwAppendNumber) > this -> e_dwMagic)
	{
		// 如果当前可用索引大于了当前容器的成员个数则表示需要扩容
		this -> expand();
	}

	// 更新成员变量
	this -> e_dwNextIndex += dwAppendNumber;		// 下一个可用索引

	// 返回此成员的指针
	T_Dynamic* pAppendElement = this -> e_pAddress + this -> e_dwNextIndex - 1;

	return pAppendElement;
}

template <class T_Dynamic> T_Dynamic* WINAPI Vector<T_Dynamic>::insert(IN DWORD dwIndexNumber)
{
	/*
		根据输入的下标在此下标处插入元素
		返回值为此插入的元素的结构体指针
	*/

	// 判断是否可以在此下标进行插入
	if ((INT)dwIndexNumber >= (INT)this -> e_dwNextIndex || (INT)dwIndexNumber < 0)
	{
		printf("--------- 不能在下标 %d 进行插入 ---------\n", dwIndexNumber);
		return NULL;
	}
	// 找到这个位置
	T_Dynamic* pInsertElement = this -> e_pAddress + dwIndexNumber;

	// 判断是否整个 Vector 可以向后移动一个单位的大小
	if (this -> e_dwNextIndex >= this -> e_dwMagic)
	{
		printf("%d\n%d\n", this -> e_dwNextIndex, this -> e_dwMagic);
		// 如果因为空间不足不能向后移动 则需要扩容
		this -> expand();

		// 更新 pInsertElement
		pInsertElement = this -> e_pAddress + dwIndexNumber;
	}

	// 计算需要拷贝的大小 从这个下标的位置开始全部向后移动一个单位大小
	memcpy(pInsertElement + 1, pInsertElement, sizeof(T_Dynamic) * (this -> e_dwNextIndex - dwIndexNumber));

	// 将插入的位置的值置零
	memset(pInsertElement, 0x00000000, sizeof(T_Dynamic));

	// 更新成员变量
	this -> e_dwNextIndex++;

	// 返回值是当前插入的元素的指针
	return pInsertElement;
}

template <class T_Dynamic> T_Dynamic* WINAPI Vector<T_Dynamic>::insert(IN DWORD dwIndexNumber, IN T_Dynamic pT_Dynamic)
{
	/*
		根据输入的下标在此下标处插入指定的元素
		返回值为此插入的元素的结构体指针
	*/

	// 判断是否可以在此下标进行插入
	if ((INT)dwIndexNumber >= (INT)this -> e_dwNextIndex || (INT)dwIndexNumber < 0)
	{
		printf("--------- 不能在下标 %d 进行插入 ---------\n", dwIndexNumber);
		return NULL;
	}
	// 找到这个位置
	T_Dynamic* pInsertElement = this -> e_pAddress + dwIndexNumber;

	// 判断是否整个 Vector 可以向后移动一个单位的大小
	if (this -> e_dwNextIndex >= this -> e_dwMagic)
	{
		printf("%d\n%d\n", this -> e_dwNextIndex, this -> e_dwMagic);
		// 如果因为空间不足不能向后移动 则需要扩容
		this -> expand();

		// 更新 pInsertElement
		pInsertElement = this -> e_pAddress + dwIndexNumber;
	}

	// 计算需要拷贝的大小 从这个下标的位置开始全部向后移动一个单位大小
	memcpy(pInsertElement + 1, pInsertElement, sizeof(T_Dynamic) * (this -> e_dwNextIndex - dwIndexNumber));

	// 将插入的位置的值改为指定的内容
	memcpy(pInsertElement, &pT_Dynamic, sizeof(T_Dynamic));

	// 更新成员变量
	this -> e_dwNextIndex++;

	// 返回值是当前插入的元素的指针
	return pInsertElement;
}

template <class T_Dynamic> T_Dynamic* WINAPI Vector<T_Dynamic>::pointerToIndex(IN DWORD dwIndexNumber)
{
	/*
		根据所给下标返回此结构的指针
	*/
	// 判断是否有这个下标
	if (dwIndexNumber >= this -> e_dwNextIndex)
	{
		printf("--------- 憨批,你输入的下标 %d 不存在 可以使用 append 函数来增加元素 ---------\n", dwIndexNumber);
		return NULL;
	}

	// 得到此下标的元素的指针并返回
	T_Dynamic* pTemp_T_Dynamic = this -> e_pAddress + dwIndexNumber;

	return pTemp_T_Dynamic;
}


template <class T_Dynamic> T_Dynamic* WINAPI Vector<T_Dynamic>::pointerToIndex(IN DWORD dwIndexNumber,OUT T_Dynamic pT_Dynamic)
{
	// 判断是否有这个下标
	if (dwIndexNumber >= this -> e_dwNextIndex)
	{
		printf("--------- 憨批,你输入的下标 %d 不存在 可以使用 append 函数来增加元素 ---------\n", dwIndexNumber);
		return NULL;
	}

	// 得到此下标的元素的指针
	T_Dynamic* pTemp_T_Dynamic = this -> e_pAddress + dwIndexNumber;

	// 将此元素的内容复制到指定位置
	memcpy(&pT_Dynamic, pTemp_T_Dynamic, sizeof(T_Dynamic));

	// 返回值为将要复制的元素的指针
	return pTemp_T_Dynamic;
}

template <class T_Dynamic> T_Dynamic* WINAPI Vector<T_Dynamic>::push(IN T_Dynamic pT_Dynamic)
{
	/* 
		在末尾插入指定的元素 
	*/

	// 判断是否空间足够
	if (this -> e_dwNextIndex >= this -> e_dwMagic)
	{
		// 如果当前可用索引大于等于了当前容器的成员个数则表示需要扩容
		this -> expand();
	}

	// 找到末尾的指针
	T_Dynamic* pPush_back = this -> e_pAddress + e_dwNextIndex;

	// 将指定的元素拷贝到数组末尾(非容器末尾)
	memcpy(pPush_back, &pT_Dynamic, sizeof(T_Dynamic));

	// 更新成员属性
	this -> e_dwNextIndex++;

	return pPush_back;

}

template <class T_Dynamic> VOID WINAPI Vector<T_Dynamic>::pop(void)
{
	/*
		将最后一个元素弹出来
		清空里面的数据
	*/

	// 找到最后一个元素的指针
	T_Dynamic* pPop_back = this -> e_pAddress + this -> e_dwNextIndex - 1;

	// 清空里面的数据
	memset(pPop_back, 0x00000000, sizeof(T_Dynamic));
	
	// 更新成员变量
	this -> e_dwNextIndex--;
	
	return; 
}

 
template <class T_Dynamic> DWORD WINAPI Vector<T_Dynamic>::capacity(void)
{
	/*
		返回在不增容的情况下，还能存储多少元素
	*/
	return this -> e_dwMagic - this -> e_dwNextIndex;
}

template <class T_Dynamic> VOID WINAPI Vector<T_Dynamic>::clear(void)
{
	/*
		清空所有元素
	*/

	// 先将内存清空
	memset(this -> e_pAddress, 0x00000000, sizeof(T_Dynamic) * this -> e_dwMagic);

	// 更新成员变量
	this -> e_dwNextIndex = NULL;

	return;
}

template <class T_Dynamic> BOOL WINAPI Vector<T_Dynamic>::IsEmpty(void)
{
	/*
		判断 Vector 是否为空 返回 true 时为空
	*/

	if (!this -> e_dwNextIndex)
	{
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> VOID WINAPI Vector<T_Dynamic>::erase(IN DWORD dwIndexNumber)
{
	/*
		根据下标删除指定元素
		删除此位置而不仅仅是清空该地址的值
	*/

	// 判断是否有此下标
	if(dwIndexNumber >= this -> e_dwNextIndex || dwIndexNumber < 0)
	{
		printf("没有这个下标\n");
		return;
	}
	
	// 找到这个位置的指针
	T_Dynamic* pErase = this -> e_pAddress + dwIndexNumber;
	
	// 将后面的元素全部向前拷贝 1 个单位大小
	memcpy(pErase, pErase + 1, sizeof(T_Dynamic) * (this -> e_dwNextIndex - dwIndexNumber - 1));

	// 将最后一个值清空
	memset(e_pAddress + this -> e_dwNextIndex - 1, 0x00000000, sizeof(T_Dynamic));

	// 更新成员变量
	this -> e_dwNextIndex--;
	
	return;
}
 
template <class T_Dynamic> DWORD WINAPI Vector<T_Dynamic>::size(void)
{
	/*
		返回 Vector 元素数量的大小
	*/

	return sizeof(T_Dynamic) * this -> e_dwNextIndex;
}

template <class T_Dynamic> VOID WINAPI Vector<T_Dynamic>::ReadVectorMemberInfo(const Vector& pVector)
{
	/* 
		读取 Vector 中全部成员的信息 并打印出来 
	*/  

	printf("当前容器的成员个数: %d\n", pVector.e_dwMagic);
	printf("下一个可用索引: %d\n", pVector.e_dwNextIndex);
	printf("默认的创建对象的个数: %d\n", pVector.e_dwLfanew);
	printf("当前容器的长度(字节): %d\n", pVector.e_dwLength);
	printf("每次增容的单位数: %d\n", pVector.e_dwIncrement);
	printf("容器首地址指针: %x\n\n", pVector.e_pAddress);

	return;
}

template <class T_Dynamic> BOOL WINAPI Vector<T_Dynamic>::operator > (const Vector& pVector)
{
	/* 
		重载 ">" 运算符
		可以使用 > 来比较两个结构体或类的大小
	*/
	if (this -> e_dwLength > pVector.e_dwLength)
	{
		// 如果 A -> e_dwLength > B.e_dwLength 则 A Vector 大于 B Vector
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI Vector<T_Dynamic>::operator >= (const Vector& pVector)
{
	/* 
		重载 ">=" 运算符
		可以使用 > 来比较两个结构体或类的大小
	*/
	if (this -> e_dwLength >= pVector.e_dwLength)
	{
		// 如果 A -> e_dwLength >= B.e_dwLength 则 A Vector 大于等于 B Vector
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI Vector<T_Dynamic>::operator < (const Vector& pVector)
{
	/* 
		重载 "<" 运算符
		可以使用 < 来比较两个结构体或类的大小
	*/
	if (this -> e_dwLength < pVector.e_dwLength)
	{
		// 如果 A -> e_dwLength < B.e_dwLength 则 A Vector 小于 B Vector
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI Vector<T_Dynamic>::operator <= (const Vector& pVector)
{
	/* 
		重载 "<=" 运算符
		可以使用 <= 来比较两个结构体或类的大小
	*/
	if (this -> e_dwLength <= pVector.e_dwLength)
	{
		// 如果 A -> e_dwLength <= B.e_dwLength 则 A Vector 小于等于 B Vector
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI Vector<T_Dynamic>::operator == (const Vector& pVector)
{
	/* 
		重载 "==" 运算符
		可以使用 == 来比较两个结构体或类的大小
	*/
	if (this -> e_dwLength == pVector.e_dwLength)
	{
		// 如果 A -> e_dwLength == B.e_dwLength 则 A Vector 等于 B Vector
		return TRUE;
	}

	return FALSE;
}

class mystruct
{
public:
	int a;
	int b;
	void func()
	{
		printf("我执行了!\n%d", this -> a);
		return;
	}
};

void function()
{
	/* 
		将 Vector 的结构成员放在栈里 
	*/

	Vector<mystruct> Example_List;
	Vector<mystruct> AAA;

	if (Example_List == AAA)
	{
		printf("这两个 Vector 的大小相等\n");
	}

	mystruct* answer1 = Example_List.append();
	answer1 -> a = 0x9;
	answer1 -> b = 0xA;

	Example_List.append(2);
	Example_List.insert(2);
	Example_List.insert(4, *answer1);

	mystruct* answer2 = Example_List.pointerToIndex(5);	
	answer2 -> a = 0x5;
	answer2 -> b = 0x6;

	Example_List.pointerToIndex(3, *answer2);

	Example_List.push(*answer1);

	Example_List.pop();

	printf("还能存储的元素数量： %d\n", Example_List.capacity());

	printf("vector的大小是：%d\n", Example_List.size());

	Example_List.ReadVectorMemberInfo(Example_List);
	
	Example_List.erase(3);

	printf("vector的大小是：%d\n", Example_List.size());

	Example_List.clear();

	if(Example_List.IsEmpty())
	{
		printf("这个 Vector 是空的\n");
	}
}

void function2()
{
	/* 
		将 Vector 的结构成员放在堆里 
	*/

	// Vector<int>* pVector_1 = new Vector<int>()
	// Vector<int>* pVector_1 = new Vector<int>(5)
	Vector<int>* pVector_1 = new Vector<int>;
	int* pa = pVector_1 -> pointerToIndex(0);
	pVector_1 -> push(1);
	pVector_1 -> push(2);
	pVector_1 -> push(3);
	pVector_1 -> push(4);
	pVector_1 -> insert(2, 10);
	pVector_1 -> push(5);
	pVector_1 -> push(6);
	pVector_1 -> push(7);
	pVector_1 -> push(8);
	pVector_1 -> push(9);
	pVector_1 -> push(10);
	int* pb = pVector_1 -> pointerToIndex(0);
	pVector_1 -> push(11);
	pVector_1 -> push(12);
	pVector_1 -> push(13);
	pVector_1 -> push(14);
	

}

int main(int argc, char* argv[])
{
	/*
		本节练习：			
			1、实现一个Vector类			
			2、读懂每一个方法的反汇编实现			
	*/

	/* 
		有几个问题需要解决：
			1.  在使用模板的类中 怎么使用友元函数 
			2.  怎么继承使用模板的类
			3.  子类继承父类后 可以通过重写父类虚函数来实现
			通过父类指针调用子类函数的功能 那么它的使用场景是在什么时候
	*/


	function();

	function2();

	getchar();

	return 0;
}


// B_CPP_13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template <class T_Dynamic> class ChainList // 一个链表的类
{
private:	// 私有结构体
	typedef struct _NODE {
		T_Dynamic			e_T_Date;									// 动态类型
		_NODE*				e_pAddress;									// 动态类型的指针 指向下一个相同的结构体
	} NODE, *PNODE;

private:	// 私有变量
	DWORD					e_dwCount;									// 链表中的元素数量
	PNODE					e_pChainListHeader;							// 链表头指针

private:	// 私有函数
	PNODE WINAPI m_GetIndexCurrentNode(IN DWORD dwIndex);				// 获取索引为 dwIndex 的指针
	PNODE WINAPI m_GetIndexPreviousNode(IN DWORD dwIndex);				// 获取索引为 dwIndex 的前一个节点指针
	PNODE WINAPI m_GetIndexNextNode(IN DWORD dwIndex);					// 获取索引为 dwIndex 的后一个节点指针	

public:		// 构造及析构函数
	WINAPI ChainList();													// 默认将创建一个对象
	WINAPI ChainList(IN DWORD dwInputeNumber);							// 根据输入的值创建对象
	WINAPI ~ChainList();												// 释放全部申请的空间

public:		// 公有函数
	BOOL  WINAPI IsEmpty(void);											// 判断链表是否为空 空返回1 非空返回0			
	BOOL  WINAPI Clear(void);											// 清空链表
	DWORD WINAPI GetElement(IN DWORD dwIndex,OUT T_Dynamic& Element);	// 根据索引获取元素
	DWORD WINAPI GetElementIndex(IN const T_Dynamic& Element);			// 根据元素获取链表中的索引
	DWORD WINAPI Append(IN T_Dynamic Element);							// 在末尾追加元素
	DWORD WINAPI Insert(IN DWORD dwIndex, IN T_Dynamic Element);		// 根据索引新增元素
	DWORD WINAPI Delete(IN DWORD dwIndex);								// 根据索引删除元素
	DWORD WINAPI GetSize(void);											// 获取链表中元素的数量

public:		// 重载 >、 >= 、 < 、 <=、 ==
	BOOL WINAPI operator > (const ChainList& pChainList);				// 重载 ">"  如果 A -> e_dwCount > B.e_dwCount 则为 TRUE
	BOOL WINAPI operator >= (const ChainList& pChainList);				// 重载 ">=" 如果 A -> e_dwCount >= B.e_dwCount 则为 TRUE
	BOOL WINAPI operator < (const ChainList& pChainList);				// 重载 "<"  如果 A -> e_dwCount < B.e_dwCount 则为 TRUE
	BOOL WINAPI operator <= (const ChainList& pChainList);				// 重载 "<=" 如果 A -> e_dwCount <= B.e_dwCount 则为 TRUE
	BOOL WINAPI operator == (const ChainList& pChainList);				// 重载 "==" 如果 A 内存和 B 内存完全相等则为 TRUE
};

template <class T_Dynamic> ChainList<T_Dynamic>::PNODE WINAPI ChainList<T_Dynamic>::m_GetIndexCurrentNode(IN DWORD dwIndex)
{
	/*
		获取索引为 dwIndex 的指针
	*/
	
	// 判断输入进来的 dwIndex 是否有效
	if (dwIndex < 0 || dwIndex >= this -> e_dwCount)
	{
		// 无效的下标
		return NULL;
	}

	// 获取链表头指针 
	ChainList<T_Dynamic>::PNODE pTempNode = this -> e_pChainListHeader;

	// 如果下标有效则获取对应的地址
	for (int i = 0; i < (INT)dwIndex; i++)
	{
		// 找到下一个地址
		pTempNode = pTempNode -> e_pAddress;
	}
	
	return pTempNode;
}

template <class T_Dynamic> ChainList<T_Dynamic>::PNODE WINAPI ChainList<T_Dynamic>::m_GetIndexPreviousNode(IN DWORD dwIndex)
{
	/*
		获取索引为 dwIndex 的前一个节点指针
	*/

	// 得到当前节点下标的前一个节点下标
	INT dwPreviousIndex = (INT)dwIndex - 1;

	// 判断输入进来的 dwIndex 的前一个节点指针是否有效
	if (dwPreviousIndex < 0 || dwPreviousIndex >= (INT)this -> e_dwCount)
	{
		// 无效的下标
		return NULL;
	}

	// 获取链表头指针 
	ChainList<T_Dynamic>::PNODE pTempNode = this -> e_pChainListHeader;

	// 如果下标有效则获取对应的地址
	for (int i = 0; i < (INT)dwPreviousIndex; i++)
	{
		// 找到下一个地址
		pTempNode = pTempNode -> e_pAddress;
	}
	
	return pTempNode;
}

template <class T_Dynamic> ChainList<T_Dynamic>::PNODE WINAPI ChainList<T_Dynamic>::m_GetIndexNextNode(IN DWORD dwIndex)
{
	/*
		获取索引为 dwIndex 的后一个节点指针
	*/

	// 得到当前节点下标的后一个节点下标
	INT dwNextIndex = (INT)dwIndex + 1;

	// 判断输入进来的 dwIndex 的前一个节点指针是否有效
	if (dwNextIndex < 0 || dwNextIndex >= (INT)this -> e_dwCount)
	{
		// 无效的下标
		return NULL;
	}

	// 获取链表头指针 
	ChainList<T_Dynamic>::PNODE pTempNode = this -> e_pChainListHeader;

	// 如果下标有效则获取对应的地址
	for (int i = 0; i < (INT)dwNextIndex; i++)
	{
		// 找到下一个地址
		pTempNode = pTempNode -> e_pAddress;
	}
	
	return pTempNode;
}

template <class T_Dynamic> WINAPI ChainList<T_Dynamic>::ChainList()
: e_dwCount(0x00000000), e_pChainListHeader(0x00000000)
{
	// 默认将申请一个元素的空间
	ChainList<T_Dynamic>::PNODE pNode = new ChainList<T_Dynamic>::NODE;
	if (!pNode)
	{
		// 内存申请失败
		printf("在无参的构造函数中 空间申请失败\n");
		return;
	}
	memset(pNode, 0x00000000, sizeof(ChainList<T_Dynamic>::NODE));

	// 更新类中的成员变量
	this -> e_dwCount = 0x00000001;
	this -> e_pChainListHeader = pNode;
}

template <class T_Dynamic> WINAPI ChainList<T_Dynamic>::ChainList(IN DWORD dwInputNumber)
: e_dwCount(0x00000000), e_pChainListHeader(0x00000000)
{
	// 将负数转化为绝对值
	if ((INT)dwInputNumber < 0x00000000)
	{
		dwInputNumber = -(INT)dwInputNumber;
	}

	// 限制输入进来的最大值
	if (dwInputNumber > 100)
	{
		dwInputNumber = 100;
	}

	// 根据输入进来的值 更新 e_dwCount 的值
	if (!dwInputNumber)
	{
		this -> e_dwCount = NULL;
		this -> e_pChainListHeader = NULL;
		return;
	}
	else
	{
		this -> e_dwCount = dwInputNumber;
	}

	// 给 ChainListNode 结构体中的地址指针赋值
	for (int i = 0; i < (INT)this -> e_dwCount; i++)
	{
		ChainList<T_Dynamic>::PNODE pTempNode;
		ChainList<T_Dynamic>::PNODE pNode = new ChainList<T_Dynamic>::NODE;
		if (!pNode)
		{
			// 内存申请失败
			printf("在带参数的构造函数中 空间申请失败\n");
			return;
		}
		memset(pNode, 0x00000000, sizeof(ChainList<T_Dynamic>::NODE));
		if (!i)
		{
			// 给链表头指针赋值
			this -> e_pChainListHeader = pNode;
		}
		if (i)
		{
			// 循环给每个结构体中的 e_pAddress 赋值为下一个结构体的首地址
			pTempNode -> e_pAddress = pNode;
		}
		pTempNode = pNode;
	}
}

template <class T_Dynamic> WINAPI ChainList<T_Dynamic>::~ChainList()
{
	// 获取链表头指针及元素个数

	// 如果链表头指针为 NULL 则无需再进行空间的释放
	if (!this -> e_pChainListHeader && !this -> e_dwCount)
	{
		// 链表为空
		return;
	}

	ChainList<T_Dynamic>::PNODE pTempNode = this -> e_pChainListHeader;
	DWORD dwTempCount = this -> e_dwCount;

	// 循环遍历全部指针释放所申请的空间
	do
	{
		if (!pTempNode -> e_pAddress)
		{
			// 如果地址的值是全0则代表链表结束
			delete[] pTempNode;
			break;
		}

		// 找到下一个地址并保存
		ChainList<T_Dynamic>::PNODE pNextNode = pTempNode -> e_pAddress;

		// 释放上一个申请的地址空间
		delete[] pTempNode;
		pTempNode = pNextNode;
	}
	// 如果 e_dwCount 等于0则代表链表结束
	while(--dwTempCount);
}

template <class T_Dynamic> BOOL WINAPI ChainList<T_Dynamic>::IsEmpty(void)
{
	/* 
		判断链表是否为空 空返回1 非空返回0 
	*/

	if (this -> e_dwCount && this -> e_pChainListHeader)
	{
		return FALSE;
	}

	return TRUE;
}

template <class T_Dynamic> BOOL WINAPI ChainList<T_Dynamic>::Clear(void)
{
	/* 
		清空链表
	*/

	// 如果链表头指针为 NULL 则无需再进行空间的释放
	if (!this -> e_pChainListHeader && !this -> e_dwCount)
	{
		// 链表为空
		printf("当前链表是空的\n");
		return TRUE;
	}

	// 获取链表头指针及元素个数
	ChainList<T_Dynamic>::PNODE pTempNode = this -> e_pChainListHeader;
	DWORD dwTempCount = this -> e_dwCount;

	// 循环遍历全部指针释放所申请的空间
	do
	{
		if (!pTempNode -> e_pAddress)
		{
			// 如果地址的值是全0则代表链表结束
			delete[] pTempNode;
			break;
		}

		// 找到下一个地址并保存
		ChainList<T_Dynamic>::PNODE pNextNode = pTempNode -> e_pAddress;

		// 释放上一个申请的地址空间
		delete[] pTempNode;
		pTempNode = pNextNode;
	}
	// 如果 e_dwCount 等于0则代表链表结束
	while(--dwTempCount);

	// 更新成员变量
	this -> e_dwCount = NULL;
	this -> e_pChainListHeader = NULL;

	return FALSE;
}

template <class T_Dynamic> DWORD WINAPI ChainList<T_Dynamic>::GetElement(IN DWORD dwIndex, OUT T_Dynamic& Element)
{
	/*
		根据索引获取元素 返回值是此元素在链表中的指针
	*/

	// 判断索引是否在合理范围
	if (dwIndex < 0 || dwIndex >= this -> e_dwCount)
	{
		// 索引不合法
		return NULL;
	}
	ChainList<T_Dynamic>::PNODE pTempNode = m_GetIndexCurrentNode(dwIndex);
	memcpy(&Element, pTempNode, sizeof(T_Dynamic));

	return TRUE;
}

template <class T_Dynamic> DWORD WINAPI ChainList<T_Dynamic>::GetElementIndex(IN const T_Dynamic& Element)
{
	/* 
		根据元素获取链表中的第一个索引 
	*/
	
	// 如果链表头指针为 NULL 则无链表为空
	if (!this -> e_pChainListHeader && !this -> e_dwCount)
	{
		// 链表为空
		printf("当前链表是空的\n");
		return 0xFFFFFFFF;
	}

	// 获取链表头指针及元素个数
	ChainList<T_Dynamic>::PNODE pTempNode = this -> e_pChainListHeader;

	// 循环遍历全部地址
	for (int ElementIndex = 0; ElementIndex < (INT)this -> e_dwCount; ElementIndex++)
	{
		// 找到第一个地址判断是否元素相等
		int result = memcmp(&Element, pTempNode, sizeof(T_Dynamic));
		if (!result)
		{
			return ElementIndex;
		}

		// 找下一个地址
		pTempNode = pTempNode -> e_pAddress;
	}
	
	printf("没有相同的元素\n");
	return 0xFFFFFFFE;
}

template <class T_Dynamic> DWORD WINAPI ChainList<T_Dynamic>::Append(IN T_Dynamic Element)
{
	/*
		在末尾追加元素 返回此元素在链表中的索引
	*/

	// 重新申请一块空间
	ChainList<T_Dynamic>::PNODE pNode = new ChainList<T_Dynamic>::NODE;

	// 初始化此空间
	memset(pNode, 0x00000000, sizeof(ChainList<T_Dynamic>::NODE));

	// 给新增的元素赋值
	memcpy(pNode, &Element, sizeof(T_Dynamic));

	if (!this -> e_dwCount && !this -> e_pChainListHeader)
	{
		// 如果链表中没有元素
		this -> e_pChainListHeader = pNode;
		this -> e_dwCount++;

		return this -> e_dwCount - 1;
	}
	
	// 找到最后一个元素的地址
	ChainList<T_Dynamic>::PNODE pTempNode = m_GetIndexPreviousNode(this -> e_dwCount);
	
	// 修正指针
	pTempNode -> e_pAddress = pNode;
	this -> e_dwCount++;

	return this -> e_dwCount - 1;
}

template <class T_Dynamic> DWORD WINAPI ChainList<T_Dynamic>::Insert(IN DWORD dwIndex, IN T_Dynamic Element)
{
	/*
		根据索引新增元素 返回此元素在链表中的索引
	*/

	// 判断索引是否在正常范围
	if ((INT)dwIndex < 0 || (INT)dwIndex > (INT)this -> e_dwCount)
	{
		// 不在合理范围则不进行空间申请
		printf("输入的下标不在合法范围内\n");
		return NULL;
	}

	// 重新申请一块空间
	ChainList<T_Dynamic>::PNODE pNode = new ChainList<T_Dynamic>::NODE;

	// 初始化此空间
	memset(pNode, 0x00000000, sizeof(ChainList<T_Dynamic>::NODE));

	// 给新增的元素赋值
	memcpy(pNode, &Element, sizeof(T_Dynamic));


	// 如果输入的下标是0
	if (!dwIndex)
	{
		this -> e_pChainListHeader = pNode;
		this -> e_dwCount++;

		return e_dwCount - 1;
	}

	// 如果输入的下标在最后一个元素之后
	if (dwIndex == this -> e_dwCount)
	{
		// 找到最后一个元素首地址
		ChainList<T_Dynamic>::PNODE pLastNode = m_GetIndexPreviousNode(this -> e_dwCount);

		// 更改最后一个元素的 e_pAddress 的指向
		pLastNode -> e_pAddress = pNode;
		this -> e_dwCount++;

		return e_dwCount - 1;
	}

	// 如果输入的下标在两元素之间
	// 获取上一个元素及下一个元素的首地址
	ChainList<T_Dynamic>::PNODE pPreviousNode = m_GetIndexPreviousNode(dwIndex);
	ChainList<T_Dynamic>::PNODE pNextNode = m_GetIndexNextNode(dwIndex);

	// 修正指向
	pPreviousNode -> e_pAddress = pNode;
	pNode -> e_pAddress = pNextNode;
	this -> e_dwCount++;

	return e_dwCount - 1;
}


template <class T_Dynamic> DWORD WINAPI ChainList<T_Dynamic>::Delete(IN DWORD dwIndex)
{
	/*
		根据索引删除元素 返回剩余的元素个数
	*/

	// 判断是否有元素
	if (!this -> e_dwCount)
	{	
		// 链表中没有元素
		printf("链表中没有元素\n");
		return NULL;
	}

	// 判断索引是否在正常范围
	if ((INT)dwIndex < 0 || (INT)dwIndex >= (INT)this -> e_dwCount)
	{
		// 不在合理范围则不进行空间申请
		printf("要删除的元素的下标不在合法范围内\n");
		return NULL;
	}

	// 根据输入的索引 找到上一个元素 / 当前元素 / 下一个元素的地址 如果没有则反返回NULL
	ChainList<T_Dynamic>::PNODE	pPreviousNode = m_GetIndexPreviousNode(dwIndex);
	ChainList<T_Dynamic>::PNODE pCurrentNode = m_GetIndexCurrentNode(dwIndex);
	ChainList<T_Dynamic>::PNODE	pNextNode = m_GetIndexNextNode(dwIndex);

	// 如果只有一个元素
	if (!(this -> e_dwCount ^ 0x00000001))
	{
		// 释放空间
		delete[]  pCurrentNode;
		pCurrentNode = NULL;

		// 更新成员变量
		this -> e_pChainListHeader = NULL;
		this -> e_dwCount = NULL;

		return this -> e_dwCount;
	}

	// 如果要删除的位置是第一个元素
	if (!(INT)dwIndex)
	{
		// 将链表头指针指向第二个元素的首地址
		this -> e_pChainListHeader = pNextNode;

		// 释放空间
		delete[]  pCurrentNode;
		pCurrentNode = NULL;

		// 更新成员变量
		this -> e_dwCount--;

		return this -> e_dwCount;
	}

	// 如果要删除的位置是最后一个元素
	if ((INT)dwIndex == this -> e_dwCount)
	{
		// 将倒数第二个元素的 e_pAddress 置零
		pPreviousNode -> e_pAddress = 0x00000000;

		// 释放空间
		delete[]  pCurrentNode;
		pCurrentNode = NULL;

		// 更新成员变量
		this -> e_dwCount--;
		
		return this -> e_dwCount;
	}

	// 如果删除的位置在两个元素之间
	// 将前一个元素指针指向后一个元素首地址
	pPreviousNode -> e_pAddress = pNextNode;

	// 释放空间
	delete[]  pCurrentNode;
	pCurrentNode = NULL;

	// 更新成员变量
	this -> e_dwCount--;
	
	return this -> e_dwCount;
}

template <class T_Dynamic> DWORD WINAPI ChainList<T_Dynamic>::GetSize(void)
{
	/*
		获取链表中元素的数量
	*/

	return this -> e_dwCount;
}

/*
	BOOL WINAPI operator == (const ChainList& pChainList);				// 重载 "==" 如果 A 内存和 B 内存完全相等则为 TRUE
*/
template <class T_Dynamic> BOOL WINAPI ChainList<T_Dynamic>::operator > (const ChainList& pChainList)
{
	/* 
		重载 ">"  如果 A -> e_dwCount > B.e_dwCount 则为 TRUE 
	*/

	if (this -> e_dwCount > pChainList.e_dwCount)
	{
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI ChainList<T_Dynamic>::operator >= (const ChainList& pChainList)
{
	/* 
		重载 ">=" 如果 A -> e_dwCount >= B.e_dwCount 则为 TRUE
	*/

	if (this -> e_dwCount >= pChainList.e_dwCount)
	{
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI ChainList<T_Dynamic>::operator < (const ChainList& pChainList)
{
	/* 
		重载 "<"  如果 A -> e_dwCount < B.e_dwCount 则为 TRUE
	*/

	if (this -> e_dwCount < pChainList.e_dwCount)
	{
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI ChainList<T_Dynamic>::operator <= (const ChainList& pChainList)
{
	/* 
		重载 "<=" 如果 A -> e_dwCount <= B.e_dwCount 则为 TRUE
	*/

	if (this -> e_dwCount <= pChainList.e_dwCount)
	{
		return TRUE;
	}

	return FALSE;
}

template <class T_Dynamic> BOOL WINAPI ChainList<T_Dynamic>::operator == (const ChainList& pChainList)
{
	/*
		重载 "==" 如果 A 内存和 B 内存完全相等则为 TRUE
	*/
	
	// 如果 A 和 B 都没有元素则相等
	if (!this -> e_dwCount && !pChainList.e_dwCount)
	{
		return TRUE;
	}

	// 如果A 和 B 元素个数不相同则不相等
	if (this -> e_dwCount != pChainList.e_dwCount)
	{
		return FALSE;
	}
	
	// 分别获取 A 和 B 链表的头地址
	ChainList<T_Dynamic>::PNODE pChainListHeader_A = this -> e_pChainListHeader;
	ChainList<T_Dynamic>::PNODE pChainListHeader_B = pChainList.e_pChainListHeader;

	// 获取元素个数
	DWORD dwTempCount = this -> e_dwCount;

	// 遍历并判断每个节点的内存是否相等
	do
	{
		if (memcmp(pChainListHeader_A, pChainListHeader_B, sizeof(T_Dynamic)))
		{
			// 如果两个元素有节点不相同则 A 和 B不相同
			return FALSE;
		}

		if (!pChainListHeader_A -> e_pAddress && !pChainListHeader_B -> e_pAddress)
		{
			// 如果 A 地址和 B 地址的值都是全0则代表链表结束
			break;
		}
		
		// 找到下一个地址
		pChainListHeader_A = pChainListHeader_A -> e_pAddress;
		pChainListHeader_B = pChainListHeader_B -> e_pAddress;

	}
	// 如果 e_dwCount 等于0则代表链表结束
	while(--dwTempCount);

	return TRUE;
}


// **********************************************************************************************


struct mystruct
{
	int a;
	int b;
};

void func()
{
	mystruct flag1;
	flag1.a = 1;
	flag1.b = 2;

	mystruct flag2;
	flag2.a = 3;
	flag2.b = 4;

	mystruct flag3;
	flag3.a = 6;
	flag3.b = 6;


	ChainList <mystruct> b1;
	b1.Append(flag1);
	b1.Append(flag2);

	ChainList <mystruct> b2;
	b2.Append(flag1);
	b2.Append(flag2);
	
	b1.Insert(2, flag3);
	b2.Insert(2, flag3);
	int ret = b2.GetElementIndex(flag3);
	printf("%d\n", ret);
	if (b1 == b2)
	{
		printf("flag1 == flag\n");
	}
	else
	{
		printf("flag1 != flag\n");
	}
	b2.GetElement(0, flag1);
	printf("%d\n%d\n", flag1.a, flag1.b);
}

int main(int argc, char* argv[])
{
	/*
		本节练习：			
			1、实现一个单项链表			
			2、读懂每一个方法的反汇编实现			
	*/
	func();

	getchar();
	return 0;
}


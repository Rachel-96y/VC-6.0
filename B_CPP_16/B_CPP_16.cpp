// B_CPP_16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template <class T_Dynamic> class BinarySearchTree								// 搜索二叉树(二叉排序树)
{	
private:		// 私有结构体
	typedef struct _NODE {														// 节点
		T_Dynamic			e_T_Date;											// 动态类型
		DWORD				e_dwID;												// 用于比较结构体大小
		_NODE*				e_pLeft;											// 左子树指针
		_NODE*				e_pRight;											// 右子树指针
		_NODE*				e_pParent;											// 父指针
	} NODE, *PNODE;

private:		// 私有变量							
	PNODE					e_pRoot;											// 根节点指针
	DWORD					e_dwCount;											// 元素总数

private:		// 私有函数
	PNODE   WINAPI   m_GetMaxNode(PNODE pNode);									// 获取以 pNode 为根的最大节点
	PNODE	WINAPI	 m_GetMinNode(PNODE pNode);									// 获取以 pNode 为根的最小节点
	PNODE	WINAPI	 m_SearchNode(PNODE pNode, T_Dynamic T_element);			// 查找指定元素(私有)
	DWORD	WINAPI	 m_InsertNode(PNODE pNode, T_Dynamic T_element);			// 新增节点(私有)
	BOOL	WINAPI	 m_DeleteNode(PNODE pNode, T_Dynamic T_element);			// 删除节点(私有)
	BOOL	WINAPI	 m_ClearNode(void);											// 清空容器

public:			// 构造及析构函数
	WINAPI   BinarySearchTree(void);											// 默认创建一个元素
	WINAPI	 BinarySearchTree(DWORD dwNumber);									// 根据输入的值创建元素
	WINAPI	 ~BinarySearchTree(void);											// 释放内存

public:			// 公有函数
	PNODE   WINAPI   Search(T_Dynamic T_element);								// 查找指定元素
	PNODE	WINAPI	 Insert(T_Dynamic T_element);								// 新增节点
	BOOL	WINAPI	 Delete(T_Dynamic T_element);								// 删除节点
	BOOL	WINAPI	 IsEmpty(void);												// 判断容器是否为空
	BOOL	WINAPI	 Clear(void);												// 清空容器

public:			// 重载 >、 >= 、 < 、 <=、 ==
	BOOL WINAPI operator > (const BinarySearchTree& pBinarySearchTree);			// 重载 ">"  如果 A -> e_dwID > B.e_dwID  则为 TRUE
	BOOL WINAPI operator >= (const BinarySearchTree& pBinarySearchTree);		// 重载 ">=" 如果 A -> e_dwID >= B.e_dwID 则为 TRUE
	BOOL WINAPI operator < (const BinarySearchTree& pBinarySearchTree);			// 重载 "<"  如果 A -> e_dwID < B.e_dwID  则为 TRUE
	BOOL WINAPI operator <= (const BinarySearchTree& pBinarySearchTree);		// 重载 "<=" 如果 A -> e_dwID <= B.e_dwID 则为 TRUE
	BOOL WINAPI operator == (const BinarySearchTree& pBinarySearchTree);		// 重载 "==" 如果 A 内存和 B 内存完全相等 则为 TRUE
};

int main(int argc, char* argv[])
{
	/* 
		实现一个自己的搜索二叉树 
	*/

	return 0;
}
// B_CPP_14.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

class Monster								
{	
	/* 
		怪物的结构体 
		存储怪物信息 【ID】【Level】【Name】
		有一个有参的构造函数 
		无参的构造函数没有进行任何操作
	*/
public:								
	int ID;							
	int Level;							
	char Name[20];							
public:								
	Monster(){}	
	Monster(int ID, int Level, char* Name)							
	{							
		this -> ID = ID;						
		this -> Level = Level;						
		memcpy(&this -> Name, Name, strlen(Name) + 1);						
	}							
};								
								
template<class T>								
class TreeNode{	
	/*
		树节点
		存储一个动态类型 + 2个地址 【pLeft】【pRight】
		有一个构造函数用于初始化节点并为元素赋值
	*/
public:								
	T element;												// 当前节点存储的数据		
	TreeNode<T>* pLeft;										// 指向左子节点的指针		
	TreeNode<T>* pRight;									// 指向右子节点的指针		
								
	TreeNode(T& ele){							
		//初始化Node节点						
		memset(&element,0,sizeof(TreeNode));						
		//为元素赋值						
		memcpy(&element,&ele,sizeof(T));						
		pLeft = pRight = NULL;						
	}							
};								
								
template<class T>								
class BSortTree{
	/*
		这是主要的类
		有二叉树的构造及析构函数
		支持多种方法对元素进行查询、新增、插入、删除等
	*/

public:								
	BSortTree();												// 构造函数		
	~BSortTree();												// 析构函数		
public:								
	void InOrderTraverse(TreeNode<T>* pNode);					// 中序遍历		
	void PreOrderTraverse(TreeNode<T>* pNode);					// 前序遍历		
	void PostOrderTraverse(TreeNode<T>* pNode);					// 后序遍历		
	TreeNode<T>* GetRoot();										// 返回根节点		
	int GetDepth(TreeNode<T>* pNode);							// 返回某个节点的高度/深度	
private:						
	void Init();												// 专属功能：为二叉树中的元素赋值【Monster】
	void ReleaseMemory(TreeNode<T>* pNode);						// 用于递归释放内存的函数
private:								
	TreeNode<T>* m_pRoot;										// 根结点指针		
	int size;													// 树中元素总个数		
};			
					
template<class T>
void BSortTree<T>::ReleaseMemory(TreeNode<T>* pNode)
{
	/*
		利用后序遍历：(左 右 根)释放内存
	*/
	// 判断是否节点是空值
	if(!pNode)
	{
		return;
	}
	
	// 递归查找
	ReleaseMemory(pNode -> pLeft);
	ReleaseMemory(pNode -> pRight);

	// 根据指针释放内存并初始化指针
	delete[] &pNode -> element;
	pNode = NULL;
}				
	
template<class T> 								
BSortTree<T>::BSortTree()								
{						
	/*
		结构体 BSortTree 的构造函数
		使用结构体类型 Moster 创建结构体对象
		并对节点元素进行赋值 
	*/
	Init();							
}								
template<class T> 								
BSortTree<T>::~BSortTree(){								
								
	/* 
		释放所有节点的空间并初始化指针 
	*/

	// 根据二叉树的根节点找到全部的节点指针并释放内存
	ReleaseMemory(m_pRoot);
	
}								
								
template<class T> 								
void BSortTree<T>::Init()								
{				
	/*
		使用结构体类型 Moster 创建结构体对象
		并对节点元素进行赋值 
	*/

	// 创建多个结构体对象并赋值【m1~m7】
	Monster m1(1, 1, "刺猬");							
	Monster m2(2, 2, "野狼");							
	Monster m3(3, 3, "野猪");							
	Monster m4(4, 4, "士兵");							
	Monster m5(5, 5, "火龙");							
	Monster m6(6, 6, "独角兽");							
	Monster m7(7, 7, "江湖大盗");							
					
	// 在堆中创建多个 TreeNode 类型的内存并对 Monster 大小的数据赋值为【m1-m7】
	TreeNode<Monster>* n1 = new TreeNode<Monster>(m1);							
	TreeNode<Monster>* n2 = new TreeNode<Monster>(m2);							
	TreeNode<Monster>* n3 = new TreeNode<Monster>(m3);							
	TreeNode<Monster>* n4 = new TreeNode<Monster>(m4);							
	TreeNode<Monster>* n5 = new TreeNode<Monster>(m5);							
	TreeNode<Monster>* n6 = new TreeNode<Monster>(m6);							
	TreeNode<Monster>* n7 = new TreeNode<Monster>(m7);							
					
	// 对二叉树节点【TreeNode】中的地址【pLeft、pRight】进行赋值
	m_pRoot = n5;							
	n5 -> pLeft = n4;							
	n5 -> pRight = n6;							
	n4 -> pLeft = n1;							
	n1 -> pRight = n2;							
	n6 -> pLeft = n3;							
	n3 -> pRight = n7;							
	size = 7;			
}
				
template<class T> 								
TreeNode<T>* BSortTree<T>::GetRoot()								
{				
	/* 
		返回根节点 
	*/
	
	return m_pRoot;							
}				
				
template<class T>								
int BSortTree<T>::GetDepth(TreeNode<T>* pNode)								
{					
	/* 
		返回某个节点的高度/深度 
	*/ 

    if(!pNode) 								
    {								
		return NULL;  							
    }								
    else  								
    {  								
        int m = GetDepth(pNode -> pLeft);  								
        int n = GetDepth(pNode -> pRight);  								
        return (m > n) ? (m+1) : (n+1);   								
    }  								
}		
						
template<class T> 								
void BSortTree<T>::InOrderTraverse(TreeNode<T>* pNode)								
{								
	/* 中序遍历所有怪物,列出怪的名字 */
	// 中序遍历：(左 根 右)

	if (!pNode)
	{
		return;
	}

	// 递归查找
	InOrderTraverse(pNode -> pLeft);

	// 找到名字的地址
	LPSTR name = (LPSTR)((DWORD)&pNode -> element + sizeof(INT) * 2);
	printf("%s\n", name);

	InOrderTraverse(pNode -> pRight);
}								
								
template<class T> 								
void BSortTree<T>::PreOrderTraverse(TreeNode<T>* pNode)								
{											
	/* 前序遍历所有怪物,列出怪的名字 */
	// 前序遍历：(根 左 右)

	if (!pNode)
	{
		return;
	}

	// 找到名字的地址
	LPSTR name = (LPSTR)((DWORD)&pNode -> element + sizeof(INT) * 2);
	printf("%s\n", name);

	// 递归查找
	PreOrderTraverse(pNode -> pLeft);
	PreOrderTraverse(pNode -> pRight);
}								
								
template<class T> 								
void BSortTree<T>::PostOrderTraverse(TreeNode<T>* pNode)								
{										
	/* 后序遍历所有怪物,列出怪的名字 */
	// 后序遍历：(左 右 根)

	if (!pNode)
	{
		return;
	}

	// 递归查找
	PostOrderTraverse(pNode -> pLeft);
	PostOrderTraverse(pNode -> pRight);

	// 找到名字的地址
	LPSTR name = (LPSTR)((DWORD)&pNode -> element + sizeof(INT) * 2);
	printf("%s\n", name);
}								


VOID function()
{
	/*
		遍历二叉树中的怪物列表
	*/
	
	// 使用 Monster 类型 创建一个 test 类型的变量
	// 包含了多个 TreeNode 类型的变量并根据二叉树的特点赋值完成
	BSortTree<Monster> test;

	// 获取二叉树的根节点
	TreeNode<Monster>* pRoot = test.GetRoot();

	// 遍历所有怪物,列出怪的名字

	printf("前序遍历:\n");
	test.PreOrderTraverse(pRoot);
	printf("\n中序遍历:\n");
	test.InOrderTraverse(pRoot);
	printf("\n后序遍历:\n");
	test.PostOrderTraverse(pRoot);

}

int main(int argc, char* argv[])
{
	/*
		本节练习：
			1、遍历二叉树中的怪物列表.		
			2、完成析构函数中的代码			
			思考题：			
			如何区分二叉树还是双向链表？		
	*/

	// 如何区分二叉树还是双向链表?
	// 看有没有一个地址指向上一个树首地址

	function();
		
	getchar();
	return 0;
}
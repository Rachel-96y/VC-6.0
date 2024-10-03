// B_CPP_15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

template<class T>
class TreeNode{	
	/*
		搜索二叉树
	*/

public:
	T element;														// 当前节点存储的数据	
	TreeNode<T>* pLeft;												// 指向左子节点的指针	
	TreeNode<T>* pRight;											// 指向右子节点的指针	
	TreeNode<T>* pParent;											// 指向父结点的指针					
							
	TreeNode(T& ele){						
		//初始化 TreeNode 节点					
		memset(&element, 0, sizeof(TreeNode));					
		//为元素赋值					
		memcpy(&element, &ele, sizeof(T));					
		pLeft = pRight = pParent = NULL;					
	}						
	//重载 == 比较两结点是否相等						
	bool operator==(TreeNode<T>* node){ 						
		return node -> element == element ? TRUE : FALSE;
	}
};

template<class T>							
class BSortTree{							
public:							
	BSortTree();													// 构造函数
	~BSortTree();													// 析构函数
public:																
	bool IsEmpty();													// 判断树是否为空
	DWORD Insert(T element);										// 新增节点
	void Delete(T element);											// 删除节点
	TreeNode<T>* Search(T element);									// 查找节点
	void InOrderTraverse(TreeNode<T>* pNode);						// 中序遍历
	void PreOrderTraverse(TreeNode<T>* pNode);						// 前序遍历
	void PostOrderTraverse(TreeNode<T>* pNode);						// 后序遍历
private:							
	TreeNode<T>* GetMaxNode(TreeNode<T>* pNode);					// 获取以 pNode 为根的最大节点
	TreeNode<T>* GetMinNode(TreeNode<T>* pNode);					// 获取以 pNode 为根的最小节点
	TreeNode<T>* SearchNode(TreeNode<T>* pNode,T element);			// 根据 element 的值搜索对应节点
	DWORD InsertNode(T element, TreeNode<T>* pNode);				// 新增节点
	TreeNode<T>* DeleteNode(T element, TreeNode<T>* pNode);			// 删除节点
	void Clear(TreeNode<T>* pNode);									// 清空所有节点
private:							
	TreeNode<T>* m_pRoot;											// 根结点指针
	int m_size;														// 树中元素总数
};				
							
template<class T> 							
BSortTree<T>::BSortTree()							
{					
	/*
		构造函数
	*/ 

	m_pRoot = NULL;	
	m_size = 0;
}			
				
template<class T>
BSortTree<T>::~BSortTree()
{							
	/*
		析构函数
	*/		

	Clear(m_pRoot);
}			
				
template<class T>
DWORD BSortTree<T>::Insert(T element)							
{		
	/*
		新增节点
	*/

	//如果根节点为空						
	if (!m_pRoot)						
	{						
		m_pRoot = new TreeNode<T>(element);					
		m_size++;					
		return TRUE;					
	}						
	//如果根节点不为空						
	return InsertNode(element, m_pRoot);						
}		
					
template<class T> 							
DWORD BSortTree<T>::InsertNode(T element, TreeNode<T>* pNode)							
{						
	/*
		新增节点(私有)
	*/

	// 将元素封装到节点中						
	TreeNode<T>* pNewNode = new TreeNode<T>(element);
	
	// 如果element == 当前节点 直接返回						
	if(element == pNode -> element)						
	{						
		return TRUE;					
	}						
	// 如果pNode的左子节点为NULL 并且element < 当前节点						
	if(pNode -> pLeft == NULL && element < pNode -> element)						
	{						
		pNode -> pLeft = pNewNode;					
		pNewNode -> pParent = pNode;					
		m_size++;					
		return TRUE;					
	}						
	// 如果pNode的右子节点为NULL 并且element > 当前节点						
	if(pNode -> pRight == NULL && element > pNode -> element)
	{						
		pNode -> pRight = pNewNode;					
		pNewNode -> pParent = pNode;					
		m_size++;					
		return TRUE;					
	}						
	// 如果element<当前节点 且当前节点的左子树不为空						
	if(element < pNode -> element)						
	{						
		InsertNode(element, pNode -> pLeft);					
	}						
	else						
	{						
		InsertNode(element, pNode -> pRight);					
	}						
	return TRUE;						
}							
							
template<class T> 							
void BSortTree<T>::Clear(TreeNode<T>* pNode)							
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
	Clear(pNode -> pLeft);
	Clear(pNode -> pRight);

	// 根据指针释放内存并初始化指针
	delete[] &pNode -> element;
	pNode = NULL;				
}							
							
template<class T> 							
bool BSortTree<T>::IsEmpty()							
{		
	/*
		判断树是否为空
	*/
	return size == 0 ? TRUE : FALSE;						
}							
							
template<class T> 							
TreeNode<T>* BSortTree<T>::Search(T element)							
{							
	return SearchNode(m_pRoot, element);						
}			
				
template<class T> 							
TreeNode<T>* BSortTree<T>::SearchNode(TreeNode<T>* pNode,T element)							
{				
	/*
		根据 element 的值搜索对应节点
	*/

	//如果节点为NULL
	if(pNode == NULL)
	{						
		return NULL;					
	}
	//如果相等	
	else if(element == pNode -> element)	
	{						
		return pNode;					
	}					
	//如果比节点的元素小 向左找	
	else if(element < pNode -> element)						
	{						
		return SearchNode(pNode -> pLeft, element);					
	}	
	//如果比节点的元素大 向右找	
	else					
	{
		return SearchNode(pNode -> pRight, element);					
	}
}							
							
template<class T> 							
void BSortTree<T>::Delete(T element)							
{							
	/*
		根据给定的元素 删除对应节点
	*/

	// 如果根节点为空 则无法进行删除
	if (!m_pRoot)
	{
		return;
	}

	// 调用私有函数进行删除
	DeleteNode(element, m_pRoot);

	return;
}
							
template<class T> 							
TreeNode<T>* BSortTree<T>::DeleteNode(T element, TreeNode<T>* pNode)							
{							
	/*
		搜索二叉树的删除功能(私有)
	*/						

	// 找到这个元素对应的首地址指针
	TreeNode<T>* pElement = SearchNode(pNode, element);

	if (!pElement)
	{
		// 如果没有找到元素
		return NULL;
	}
			
	// 如果容器中只有一个元素
	if (m_size == 1)
		{	
			// 删除此元素
			delete[] pElement;

			// 初始化指针
			pElement = NULL;

			// 更改成员属性
			m_pRoot = NULL;												
			m_size = NULL;											

			return NULL;
		}

	// 如果是叶子节点(没有子树)
	if (!pElement -> pLeft && !pElement -> pRight)
	{
		// 找到父节点对应的指针并置为 NULL
		if (pElement -> element < pElement -> pParent -> element)
		{
			pElement -> pParent -> pLeft = NULL;
		}
		else
		{
			pElement -> pParent -> pRight = NULL;
		}

		// 释放此节点对应的空间
		delete[] pElement;

		// 初始化指针
		pElement = NULL;

		// 更新成员变量
		m_size--;

		return NULL;
	}

	// 如果有一个子树
	if (pElement -> pLeft && !pElement -> pRight)
	{
		// 将父节点对应的指针指向子树
		if (pElement -> element < pElement -> pParent -> element)
		{
			pElement -> pParent -> pLeft = pElement -> pLeft;
		}
		else
		{
			pElement -> pParent -> pRight = pElement -> pLeft;
		}

		// 释放当前节点的空间
		delete pElement;

		// 初始化指针
		pElement = NULL;

		// 更新成员变量
		m_size--;

		return NULL;
	}
	else if (!pElement -> pLeft && pElement -> pRight)
	{
		if (pElement -> element < pElement -> pParent -> element)
		{
			pElement -> pParent -> pLeft = pElement -> pRight;
		}
		else
		{
			pElement -> pParent -> pRight = pElement -> pRight;
		}

		// 释放当前节点的空间
		delete pElement;

		// 初始化指针
		pElement = NULL;

		// 更新成员变量
		m_size--;

		return NULL;
	}

	// 如果有两个子树
	if (pElement -> pLeft && pElement -> pRight)
	{
		// 找到右子树中最小的节点
		TreeNode<T>* pMinNode = GetMinNode(pElement -> pRight);

		// 将右子树最小的节点的值赋值给当前节点
		pElement -> element = pMinNode -> element;

		// 将右子树中最小的节点的父节点的指针指向它的子树
		if (!pMinNode -> pRight)
		{
			pMinNode -> pParent -> pRight = NULL;
		}
		else if (pMinNode -> pRight)
		{
			pMinNode -> pParent -> pRight = pMinNode -> pRight;
		}

		// 释放当前节点的空间
		delete[] pMinNode;

		// 初始化指针
		pMinNode = NULL;

		// 更新成员变量
		m_size--;

		return NULL;
	}
}
			
template<class T>
TreeNode<T>* BSortTree<T>::GetMinNode(TreeNode<T>* pNode)
{
	/* 
		获取以 pNode 为根的右子树的最小节点
	*/ 

	if (!pNode -> pLeft)
	{
		return pNode;
	}

	GetMinNode(pNode -> pLeft);
	
}


// ***********************************************************************						


// 测试代码：							
			
void TestInsert()							
{							
	//12 8 5 9 17 15 13
	
	/*						
				12			
							
		8				17	
							
	5		9		15		
							
				13			
							
	*/						
							
	BSortTree<int> tree;						
							
	tree.Insert(12);						
	tree.Insert(8);						
	tree.Insert(5);						
	tree.Insert(9);						
	tree.Insert(17);						
	tree.Insert(15);						
	tree.Insert(13);						
}
						
void TestSerch()							
{							
	//12 8 5 9 17 15 13						
							
	BSortTree<int> tree;						
							
	tree.Insert(12);						
	tree.Insert(8);						
	tree.Insert(5);						
	tree.Insert(9);						
	tree.Insert(17);						
	tree.Insert(15);						
	tree.Insert(13);						
							
	TreeNode<int>* p = tree.Search(17);
	if (!p)
	{
		printf("没有此元素\n");
		return;
	}
							
	printf("%x %d\n", p, p -> element);						
}							

void TestDelete()							
{							
	//12 8 5 9 17 15 13						
	/*						
				12			
							
		8				17	
							
	5		9		15		
							
				13			
							
	*/						
							
	BSortTree<int> tree;						
							
	tree.Insert(12);						
	tree.Insert(8);						
	tree.Insert(5);						
	tree.Insert(9);						
	tree.Insert(17);						
	tree.Insert(15);						
	tree.Insert(13);

	// tree.Delete(12);
	tree.Delete(8);
	tree.Delete(5);
	tree.Delete(9);
	tree.Delete(17);
	tree.Delete(13);
	tree.Delete(15);





	
	TreeNode<int>* ret = tree.Search(12);
	printf("%x\n", ret);
	ret = tree.Search(8);
	printf("%d\n", ret);
	ret = tree.Search(5);
	printf("%d\n", ret);
	ret = tree.Search(9);
	printf("%d\n", ret);
	ret = tree.Search(17);
	printf("%d\n", ret);
	ret = tree.Search(15);
	printf("%d\n", ret);
	ret = tree.Search(13);
	printf("%d\n", ret);

	return;
}	

int main(int argc, char* argv[])
{
	/*
		本节练习：
		完成搜索二叉树的删除功能
	*/

	TestDelete();
	
	getchar();
	return 0;
}

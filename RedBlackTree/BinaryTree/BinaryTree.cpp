#include <Windows.h>
#include "BinaryTree.h"
HANDLE hc;
CONSOLE_SCREEN_BUFFER_INFO csbi;
RedBlackBSearchTree::RedBlackBSearchTree()
{
	hc = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hc, &csbi);
	m_iNodeCount = 0;

	Nil = new st_TNODE;
	Nil->Color = BLACK;
	Nil->pParent = NULL;
	Nil->pLeft = NULL;
	Nil->pRight = NULL;

	m_pRoot = Nil;
}

RedBlackBSearchTree::~RedBlackBSearchTree()
{
	ReleaseTree();
}

bool RedBlackBSearchTree::InsertNode(int iData)
{
	st_TNODE* newNode = new st_TNODE;
	newNode->iData = iData;
	newNode->pLeft = Nil;
	newNode->pRight = Nil;
	newNode->Color = RED;
	newNode->pParent = Nil;

	if (m_pRoot == Nil)
	{
		m_pRoot = newNode;
		newNode->Color = BLACK;
		m_iNodeCount++;
	}
	else
		 linkNode(m_pRoot, newNode);

	// ���⼭ �뷱�� �����Ѵ�.
	InsertSort(newNode);

	return true;
}

bool RedBlackBSearchTree::linkNode(st_TNODE *pParent, st_TNODE *pChild)
{
	if (pParent == nullptr)
		return false;

	pChild->pParent = pParent;

	if (pParent->iData > pChild->iData)
	{
		if (pParent->pLeft == Nil)
			pParent->pLeft = pChild;
		else
			return linkNode(pParent->pLeft, pChild);
	}
	else
	{
		if (pParent->pRight == Nil)
			pParent->pRight = pChild;
		else
			return linkNode(pParent->pRight, pChild);
	}

	m_iNodeCount++;
	return true;
}

bool RedBlackBSearchTree::DeleteNode(int iData)
{
	if (deleteNode(m_pRoot, Nil, iData) == Nil)
		return false;

	return true;
}

st_TNODE* RedBlackBSearchTree::deleteNode(st_TNODE *pNode, st_TNODE *pParent, int iData)
{
	if (pNode == Nil)
		return nullptr;

	if (iData != pNode->iData)
	{
		if (pNode->iData > iData)
			return deleteNode(pNode->pLeft, pNode, iData);
		else 
			return deleteNode(pNode->pRight, pNode, iData);
	} 
	else
	{
		st_TNODE* delNode = new st_TNODE;
		delNode->iData = iData;
		delNode->pLeft = pNode->pLeft;
		delNode->pRight = pNode->pRight;
		delNode->Color = pNode->Color;

		// �ش� ���  pNode ����
		if (pNode->pLeft == Nil && pNode->pRight == Nil)
		{
			//  �׳� ����
			if (pParent->pLeft == pNode)
				pParent->pLeft = Nil;
			else
				pParent->pRight = Nil;

			delete pNode;
			Nil->pParent = pParent;

			if (delNode->Color != RED)
				// ���� ����� �ڽ��� Replace ���� 
				DeleteSort(delNode->pLeft);
			
			Nil->pParent = nullptr;

			m_iNodeCount--;
			return delNode;
			
		}
		else if (pNode->pLeft == Nil || pNode->pRight == Nil)
		{
			// �������� �ڽ��� ������ �����Ѵٸ� �� ��带 Replace�� �ؾߵȴ�.
			st_TNODE *child = nullptr;
			if (pNode->pLeft != Nil)
			{
				child = pNode->pLeft;
				if (pParent->pLeft == pNode)
					pParent->pLeft = child;
				else 
					pParent->pRight = child;
			}
			else
			{
				child = pNode->pRight;
				if (pParent->pLeft == pNode)
					pParent->pLeft = child;
				else
					pParent->pRight = child;
			}
			
			delete pNode;
			

			if (delNode->Color != RED)
				DeleteSort(child);

			m_iNodeCount--;
			return delNode;
			
		}
		else
		{
			st_TNODE *pTNode = pNode->pRight;

			while (pTNode->pLeft != Nil)
				pTNode = pTNode->pLeft;
			
			pNode->iData = pTNode->iData;
			delNode->iData = pTNode->iData;

			
			delete pTNode;
			
			if (delNode->Color != RED)
			{
				if (pTNode->pRight != Nil)
					DeleteSort(pTNode->pRight);
				else
					DeleteSort(pTNode->pLeft);
			}
			m_iNodeCount--;
			return delNode;
		}

	}

}
st_TNODE* RedBlackBSearchTree::SearchNode(int iData)
{
	return findNode(m_pRoot, iData);
}

st_TNODE* RedBlackBSearchTree::findNode(st_TNODE *pNode, int iData)
{
	if (pNode == Nil)
		return Nil;

	if (pNode->iData == iData)
		return pNode;

	if (pNode->iData > iData)
		return findNode(pNode->pLeft, iData);
	return findNode(pNode->pRight, iData);

}


bool RedBlackBSearchTree::ReleaseTree(void)
{
	releaseNode(m_pRoot);

	if (Nil != nullptr)
		delete Nil;

	return true;
}

void RedBlackBSearchTree::releaseNode(st_TNODE *pNode)
{
	if (pNode == nullptr)
		return;

	if (pNode == Nil)
		return;

	releaseNode(pNode->pLeft);
	releaseNode(pNode->pRight);
	delete pNode;
}


void RedBlackBSearchTree::PreorderPrint(st_TNODE *pNode , int iDepth , int iLR )
{
	if (m_pRoot == nullptr)
		return;

	if (iDepth > 0 && pNode == nullptr)
		return;

	st_TNODE *traversalNode = nullptr;

	if (pNode == nullptr)
		traversalNode = m_pRoot;
	else
		traversalNode = pNode;

	if (traversalNode == Nil)
		return;
	
	SetConsoleTextAttribute(hc, traversalNode->Color == RED ? 12 : 15);
	for (int i = 0; i < iDepth; i++)
		printf("\t");
	printf("%d\n", traversalNode->iData);

	PreorderPrint(traversalNode->pLeft, iDepth + 1, iLR + 1);
	PreorderPrint(traversalNode->pRight, iDepth + 1, iLR - 1);
	SetConsoleTextAttribute(hc, 7);
}


void RedBlackBSearchTree::rotateL(st_TNODE *pParentNode)
{
	if (pParentNode == Nil)
		return;

	st_TNODE* GrandNode = pParentNode->pParent;
	st_TNODE* rightChild = pParentNode->pRight;
	
	// 1. ������ �ִ� �ڽ� ��带 �θ���� �����Ѵ�. pNode��ġ�� �θ���
	rightChild->pParent = GrandNode;
	pParentNode->pParent = rightChild;
	// ������ �ڽĳ���� ���� �ڽ� ��带 �θ��� ������ �ڽ����� ����
	pParentNode->pRight = rightChild->pLeft;
	rightChild->pLeft = pParentNode;

	if (GrandNode != Nil)
	{
		if (GrandNode->pLeft->iData == pParentNode->iData)
			GrandNode->pLeft = rightChild;
		else
			GrandNode->pRight = rightChild;
	}
}

void RedBlackBSearchTree::rotateR(st_TNODE *pParentNode)
{
	if (pParentNode == Nil)
		return;

	st_TNODE* GrandNode = pParentNode->pParent;
	st_TNODE* leftChild = pParentNode->pLeft;

	leftChild->pParent = GrandNode;
	pParentNode->pParent = leftChild;

	pParentNode->pLeft = leftChild->pRight;
	leftChild->pRight = pParentNode;

	if (GrandNode != Nil)
	{
		if (GrandNode->pLeft->iData == pParentNode->iData)
			GrandNode->pLeft = leftChild;
		else
			GrandNode->pRight = leftChild;
	}
}

void RedBlackBSearchTree::InsertSort(st_TNODE *pNode)
{
	// 4���� ������Ȳ
	/*
		1. ���� �θ� ���̵� ����.
		2. ���� ������ ����, �θ� ����, ������ ��.
		3. ���� ���� ����, �θ� ����, ������ ��.
	*/
	if (pNode == m_pRoot)
		return;

	st_TNODE *pParent = pNode->pParent;
	st_TNODE *pGrand = pParent->pParent;
	st_TNODE *pUncle = nullptr;
	if (pParent == m_pRoot)
		return;

	if (pGrand == nullptr)
		return;
	
	if (pParent == pGrand->pLeft)
		pUncle = pGrand->pRight;
	else
		pUncle = pGrand->pLeft;

	// 1. ���� �θ� ���̵� �����϶�
	if (pNode->Color == RED && pParent->Color == RED && pUncle->Color == RED)
	{
		// �θ�� ������ ������ �ٲ۴�.
		pParent->Color = BLACK;
		pUncle->Color = BLACK;
		// �Ҿƹ����� ����� �ٲ۴�.
		pGrand->Color = RED;
		// �Ҿƹ����� �������� ����Ѵ�. 
		InsertSort(pGrand);
	}
	// ���� ������ ����, �θ� ����, ������ ��
	else if (pNode->Color == RED && pParent->pRight == pNode &&
		pParent->Color == RED && pUncle->Color == BLACK)
	{
		if ((pGrand->pLeft == pParent && pParent->pLeft == pNode) ||
			((pGrand->pRight == pParent && pParent->pRight == pNode)))
		{
			rotateL(pGrand);
			//pNode->Color = BLACK;
			pParent->Color = BLACK;
			pGrand->Color = RED;
		}
		// �θ�������� ��ȸ��
		else
		{
			rotateL(pParent);
			// 3�� ��Ȳ���� �ٲ��.
		}
		InsertSort(pParent);
	}
	else if (pNode->Color == RED && pParent->pLeft == pNode &&
		pParent->Color == RED && pUncle->Color == BLACK)
	{
		pParent->Color = BLACK;
		pGrand->Color = RED;
		rotateR(pGrand);
	}

	while (m_pRoot->pParent != Nil)
		m_pRoot = m_pRoot->pParent;


	m_pRoot->Color = BLACK;
}
void RedBlackBSearchTree::DeleteSort(st_TNODE *pReplace)
{
	while (1)
	{
		// Replace ��尡 RED���
		if (pReplace->Color == RED)
		{
			pReplace->Color = BLACK;
			break;
		}

		//  Replace�� ��Ʈ���
		if (pReplace == m_pRoot)
		{
			pReplace->Color = BLACK;
			break;
		}

		st_TNODE *pParent = pReplace->pParent;
		st_TNODE *pSibling = nullptr;

		if (pParent->pLeft == pReplace)
			pSibling = pParent->pRight;
		else
			pSibling = pParent->pLeft;

		// 3. ���� ��尡 ������ ��Ȳ
		if (pSibling->Color == RED)
		{
			// ���� ��尡 ������ ��Ȳ��, ������ �ٲ۵�.
			pSibling->Color = BLACK;
			// �θ� ����� �ٲٰ�
			pParent->Color = RED;
			// �θ�������� ��ȸ���� �Ѵ�.
			rotateL(pParent);
			// �׸��� Replace �������� ó������ �뷱�� �۾��� ����.
		}
		// ������ �� + ������ �� �ڽ��� ��
		else if (pSibling->Color == BLACK && pSibling->pLeft->Color == BLACK &&  pSibling->pRight->Color == BLACK)
		{
			// ������ ������ �����
			pSibling->Color = RED;
			//???? �θ�������� (Replace�� �θ��)
			pReplace = pReplace->pParent;
		}
		// ���簡 �� + ������ �� �ڽ��� ����
		else if (pSibling->Color == BLACK && pSibling->pLeft->Color == RED)
		{
			// ������ ������ ������ ���ڽ��� ������ �ٲ۴�.
			pSibling->pLeft->Color = BLACK;
			// ������ ����� �ٲ۴�.
			pSibling->Color = RED;
			// ������ ��ȸ���Ѵ�.
			rotateR(pSibling);
			PreorderPrint();
		}
		else if (pSibling->Color == BLACK && pSibling->pRight->Color == RED)
		{
			// ������ �θ�� ���� ������ �ٲ��ش�.
			pSibling->Color = pParent->Color;
			pParent->Color = BLACK;
			// ������ �����ڽ��� ��
			pSibling->pRight->Color = BLACK;
			// �θ�������� ��ȸ��
			rotateL(pParent);
			PreorderPrint();
			break;
		}
	}
	
	m_pRoot->Color = BLACK;
}
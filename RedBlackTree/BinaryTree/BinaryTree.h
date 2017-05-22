#pragma once

#include <stdio.h>
#include <iostream>
/*
	  삽입기준 - 작은값은 왼쪽, 큰값은 오른쪽 으로 삽입
*/
enum NODE_COLOR
{
		BLACK = 0,
		RED
};

struct st_TNODE
{
	struct st_TNODE *pParent;
	struct st_TNODE *pLeft;
	struct st_TNODE *pRight;

	NODE_COLOR Color;

	int iData;
};


class RedBlackBSearchTree
{
public:

	RedBlackBSearchTree();
	~RedBlackBSearchTree();

	//------------------------------------------------------
	// 새로운 데이터를 추가한다.
	//------------------------------------------------------
	bool		InsertNode(int iData);
	//------------------------------------------------------
	// 특정 데이터를 삭제한다.
	//------------------------------------------------------
	bool		DeleteNode(int iData);

	//------------------------------------------------------
	// 특정 데이터를 검색하여 노드 리턴
	//------------------------------------------------------
	st_TNODE	*SearchNode(int iData);

	//------------------------------------------------------
	// 트리 전체 삭제
	//------------------------------------------------------
	bool		ReleaseTree(void);

	//------------------------------------------------------
	// 전위순회 방식 출력
	//------------------------------------------------------
	void		PreorderPrint(st_TNODE *pNode = NULL, int iDepth = 0, int iLR = 0);



private:

	//------------------------------------------------------
	// InsertNode 내부에서 호출되는 위치 찾아 추가 재귀 함수
	//------------------------------------------------------
	bool		linkNode(st_TNODE *pParent, st_TNODE *pChild);

	//------------------------------------------------------
	// SearchNode 내부에서 호출되는 노드찾기 재귀함수
	//------------------------------------------------------
	st_TNODE *	findNode(st_TNODE *pNode, int iData);

	//------------------------------------------------------
	// DeleteNode 내부에서 호출되는 노드찾기 & 삭제 & 후처리 재귀함수
	//------------------------------------------------------
	st_TNODE *	deleteNode(st_TNODE *pNode, st_TNODE *pParent, int iData);

	//------------------------------------------------------
	// ReleaseTree 재귀적으로 삭제
	//------------------------------------------------------
	void		releaseNode(st_TNODE *pNode);

	//------------------------------------------------------
	// pNode 기준으로 Left 회전
	//------------------------------------------------------
	void		rotateL(st_TNODE *pNode);

	//------------------------------------------------------
	// pNode 기준으로 Right 회전
	//------------------------------------------------------
	void		rotateR(st_TNODE *pNode);

	//------------------------------------------------------
	// InsertNode에서 노드를 넣은후 정렬하는 함수
	//------------------------------------------------------
	void		InsertSort(st_TNODE *pNode);


	void		DeleteSort(st_TNODE *pNode);

	st_TNODE* m_pRoot;
	st_TNODE* Nil;			// 끝 리프노드
	int			m_iNodeCount;

};

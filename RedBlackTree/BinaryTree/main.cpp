#include "BinaryTree.h"
#include <conio.h>  
#include <iostream>
int main()
{
	RedBlackBSearchTree Tree;

	while (1)
	{
		int selint = 0;
		std::cout << "Insert Number (-1 is cancle) : ";
		std::cin >> selint;
		system("cls");
		if(selint != -1)
			Tree.InsertNode(selint);

		Tree.PreorderPrint();


		std::cout << "Delete Number (-1 is cancle) : ";
		std::cin >> selint;

		Tree.DeleteNode(selint);

		system("cls");
		Tree.PreorderPrint();
	}

	_getch();

	return 1;
}
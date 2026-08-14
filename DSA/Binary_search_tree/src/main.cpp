#include <iostream>
#include "BinarySearchTree.h"

int main()
{
    BinarySearchTree bst;

    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    std::cout << "Preorder : ";
    bst.preorder();

    std::cout << "Inorder  : ";
    bst.inorder();

    std::cout << "Postorder: ";
    bst.postorder();

    std::cout << std::endl;

    std::cout << "Min = " << bst.findMin() << std::endl;

    int value = 60;

    if (bst.search(value))
        std::cout << value << " Found" << std::endl;
    else
        std::cout << value << " Not Found" << std::endl;

    value = 100;

    if (bst.search(value))
        std::cout << value << " Found" << std::endl;
    else
        std::cout << value << " Not Found" << std::endl;

    return 0;
}
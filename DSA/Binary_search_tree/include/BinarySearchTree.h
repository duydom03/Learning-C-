#pragma once

#include "node.h"

class BinarySearchTree
{
private:
    Node* root;

    Node* insert(Node* node, int value);
    Node* search(Node* node, int value);
    Node* findMin(Node* node);

    void preorder(Node* node);
    void inorder(Node* node);
    void postorder(Node* node);

    void destroyTree(Node* node);

    

public:
    BinarySearchTree();

    ~BinarySearchTree();

    void insert(int value);

    bool search(int value);

    void preorder();

    void inorder();

    void postorder();

    int findMin();
};


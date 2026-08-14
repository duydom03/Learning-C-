#include <iostream>
#include "BinarySearchTree.h"

using namespace std;

BinarySearchTree::BinarySearchTree()
{
    root = nullptr;
}

BinarySearchTree::~BinarySearchTree()
{
    destroyTree(root);
}

void BinarySearchTree::destroyTree(Node* node)
{
    if (node == nullptr)
        return;

    destroyTree(node->left);
    destroyTree(node->right);

    delete node;
}

Node* BinarySearchTree::insert(Node* node, int value)
{
    if (node == nullptr)
        return new Node(value);

    if (value < node->data)
        node->left = insert(node->left, value);

    else if (value > node->data)
        node->right = insert(node->right, value);

    return node;
}

void BinarySearchTree::insert(int value)
{
    root = insert(root, value);
}

Node* BinarySearchTree::search(Node* node, int value)
{
    if (node == nullptr)
        return nullptr;

    if (node->data == value)
        return node;

    if (value < node->data)
        return search(node->left, value);

    return search(node->right, value);
}

bool BinarySearchTree::search(int value)
{
    return search(root, value) != nullptr;
}

void BinarySearchTree::preorder(Node* node)
{
    if (node == nullptr)
        return;

    cout << node->data << " ";

    preorder(node->left);
    preorder(node->right);
}

Node* BinarySearchTree::findMin(Node* node)
{
    if (node == nullptr)
        return nullptr;

    while (node->left != nullptr)
    {
        node = node->left;
    }

    return node;
}

int BinarySearchTree::findMin()
{
    Node* minNode = findMin(root);

    if (minNode == nullptr)
        throw std::runtime_error("Tree is empty.");

    return minNode->data;
}

void BinarySearchTree::preorder()
{
    preorder(root);
    cout << endl;
}

void BinarySearchTree::inorder(Node* node)
{
    if (node == nullptr)
        return;

    inorder(node->left);

    cout << node->data << " ";

    inorder(node->right);
}

void BinarySearchTree::inorder()
{
    inorder(root);
    cout << endl;
}

void BinarySearchTree::postorder(Node* node)
{
    if (node == nullptr)
        return;

    postorder(node->left);

    postorder(node->right);

    cout << node->data << " ";
}

void BinarySearchTree::postorder()
{
    postorder(root);
    cout << endl;
}
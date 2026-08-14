#include <iostream>

class Node
{
public:
    int data;
    Node* next;

    Node(int value)
        : data(value), next(nullptr)
    {
    }
};

class Stack
{
private:
    Node* topNode;

public:
    Stack()
        : topNode(nullptr)
    {
    }

    ~Stack()
    {
        while (!empty())
            pop();
    }

    void push(int value)
    {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop()
    {
        if (empty())
            return;

        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    int top() const
    {
        return topNode->data;
    }

    bool empty() const
    {
        return topNode == nullptr;
    }

    int size() const
    {
        int count = 0;
        Node* cur = topNode;

        while (cur != nullptr)
        {
            count++;
            cur = cur->next;
        }

        return count;
    }
};

int main()
{
    Stack s;

    std::cout << "===== Stack Linked List Demo =====\n\n";

    // Kiểm tra stack rỗng
    std::cout << "Stack rong? "
              << (s.empty() ? "Yes" : "No") << std::endl;

    // Push
    std::cout << "\nPush 10\n";
    s.push(10);

    std::cout << "Push 20\n";
    s.push(20);

    std::cout << "Push 30\n";
    s.push(30);

    std::cout << "Top hien tai: " << s.top() << std::endl;

    std::cout << "Size: " << s.size() << std::endl;

    // Pop
    std::cout << "\nPop 1 lan\n";
    s.pop();

    std::cout << "Top sau khi pop: "
              << s.top() << std::endl;

    std::cout << "Size: "
              << s.size() << std::endl;

    // Push tiếp
    std::cout << "\nPush 40\n";
    s.push(40);

    std::cout << "Top hien tai: "
              << s.top() << std::endl;

    std::cout << "Size: "
              << s.size() << std::endl;

    // Pop toàn bộ
    std::cout << "\nXoa toan bo Stack:\n";

    while (!s.empty())
    {
        std::cout << "Top = "
                  << s.top()
                  << " -> Pop\n";

        s.pop();
    }

    std::cout << "\nStack rong? "
              << (s.empty() ? "Yes" : "No")
              << std::endl;

    return 0;
}
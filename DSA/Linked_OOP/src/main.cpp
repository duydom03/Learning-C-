#include <iostream>
using namespace std;

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

class LinkedList
{
private:
    Node* head;

public:
    LinkedList()
        : head(nullptr)
    {
    }

    ~LinkedList()
    {
        while(head != nullptr)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_front(int value)
    {
        Node* newNode = new Node(value);

        newNode->next = head;

        head = newNode;
    }

    void push_back(int value)
    {
        Node* newNode = new Node(value);

        if(head == nullptr)
        {
            head = newNode;
            return;
        }

        Node* cur = head;

        while(cur->next != nullptr)
        {
            cur = cur->next;
        }

        cur->next = newNode;
    }

    void insert(int pos, int value)
    {
        if(pos == 0)
        {
            push_front(value);
            return;
        }

        Node* cur = head;

        for(int i = 0; i < pos - 1 && cur != nullptr; i++)
        {
            cur = cur->next;
        }

        if(cur == nullptr)
            return;

        Node* newNode = new Node(value);

        newNode->next = cur->next;

        cur->next = newNode;
    }

    void erase(int pos)
    {
        if(head == nullptr)
            return;

        if(pos == 0)
        {
            Node* del = head;
            head = head->next;
            delete del;
            return;
        }

        Node* cur = head;

        for(int i = 0; i < pos - 1 && cur != nullptr; i++)
        {
            cur = cur->next;
        }

        if(cur == nullptr || cur->next == nullptr)
            return;

        Node* del = cur->next;

        cur->next = del->next;

        delete del;
    }

    void print() const
    {
        Node* cur = head;

        while(cur != nullptr)
        {
            cout << cur->data << " ";
            cur = cur->next;
        }

        cout << endl;
    }
};

int main()
{
    LinkedList list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(70);
    list.push_back(80);
    list.push_back(90);

    cout<<"Linked_Lists : "<< " "<<endl;
    list.print();

    list.push_front(5);

    list.print();

    list.insert(3,50);

    list.print();

    list.erase(5);

    list.print();

    return 0;
}
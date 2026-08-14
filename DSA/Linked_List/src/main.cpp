#include <iostream>


// // Node class to represent a node of the linked list.
// class Node {
//   public:
//     int data;
//     Node *next;

//     // Default constructor
//     Node() {
//         data = 0;
//         next = NULL;
//     }

//     // Parameterised Constructor
//     Node(int data) {
//         this->data = data;
//         this->next = NULL;
//     }
// };

class Node{
    public:
    int data;
    Node* next;

    Node(int value){
        data = value;
        next = nullptr;
    }
};

void insertFont(Node*& head , int value){
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
}

void insertBack(Node*& head , int value){
    Node* newNode = new Node(value);

    if(head == nullptr){
        head = newNode;
        return;
    }

    Node* temp = head;

    while (temp->next != nullptr)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    
}

void insertAt(Node*& head , int position , int value){
    if (position == 0)
    {
        insertFont(head , value);
        return;
    }

    Node* temp = head;

    for(int i=0; i < position - 1 && temp != nullptr; i++){
        temp = temp->next;
    }

    if (temp == nullptr)
    {
        return;
    }

    Node* newNode = new Node(value);
    newNode->next = temp->next;
    temp->next = newNode; 
}

void deleteFront(Node*& head){
    if (head == nullptr)
    {
        return;
    }

    Node* temp = head;
    head = head->next;
    delete temp;
}

void deleteBack(Node*& head){
    if (head == nullptr)
    {
        return;
    }

    if (head->next == nullptr)
    {
        delete head;
        head = nullptr;
        return;
    }

    Node* temp = head;

    while (temp->next->next != nullptr)
    {
        temp = temp->next;
    }

    delete temp->next;
    temp->next = nullptr;
}

void deleteAt(Node*& head , int position){
    if (head == nullptr) return;
   
    if (position == 0)
    {
        deleteFront(head);
        return;
    }

    Node* temp = head;

    for(int i = 0; i < position - 1 && temp != nullptr ; i++ ){
        temp = temp->next;
    }

    if (temp == nullptr || temp->next == nullptr) return ;
    
    Node* del = temp->next;
    temp->next = del->next;
    delete del ;   
}

void printList(Node* head){
    Node* temp = head;

    while (temp != nullptr)
    {
        std::cout<< temp->data << " ";
        temp = temp->next;
    }
    std::cout<<std::endl;
}

int main(){
    Node* head = nullptr;

    insertFont(head , 10);

    std::cout<<"Ban dau : ";
    printList(head);

    insertBack(head , 20);
    insertBack(head , 30);
    insertBack(head , 40);

    std::cout<<"List : ";
    printList(head);

    insertAt(head , 3 , 35);
    std::cout<<"Insert a element 35 in position 3 : ";
    printList(head);

    deleteAt(head , 3);
    std::cout<<"Delete a element in position 3 : ";
    printList(head);

    deleteBack(head);
    std::cout<<"Delete a last element  : ";
    printList(head);

    deleteFront(head);
    std::cout<<"Delete a first element : ";
    printList(head);

    return 0;
}
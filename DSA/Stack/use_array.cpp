#include <iostream>

class Stack
{
private:
    int arr[100];
    int topIndex;

public:
    Stack() : topIndex(-1) {}

    void push(int value)
    {
        if (topIndex == 99)
        {
            std::cout << "Stack Overflow\n";
            return;
        }

        arr[++topIndex] = value;
    }

    void pop()
    {
        if (empty())
            return;

        topIndex--;
    }

    int top() const
    {
        return arr[topIndex];
    }

    bool empty() const
    {
        return topIndex == -1;
    }

    int size() const
    {
        return topIndex + 1;
    }
};


int main()
{
    Stack s;

    std::cout << "===== Stack Array Demo =====\n\n";

    // Kiểm tra stack ban đầu
    std::cout << "Stack rong? "
         << (s.empty() ? "Yes" : "No") << std::endl;

    // Push các phần tử
    std::cout << "\nPush 10" << std::endl;
    s.push(10);

    std::cout << "Push 20" << std::endl;
    s.push(20);

    std::cout << "Push 30" << std::endl;
    s.push(30);

    // Hiển thị thông tin
    std::cout << "\nTop hien tai: " << s.top() << std::endl;
    std::cout << "Size: " << s.size() << std::endl;

    // Pop một phần tử
    std::cout << "\nPop 1 lan" << std::endl;
    s.pop();

    std::cout << "Top sau khi pop: " << s.top() << std::endl;
    std::cout << "Size: " << s.size() << std::endl;

    // Push tiếp
    std::cout << "\nPush 40" << std::endl;
    s.push(40);

    std::cout << "Top hien tai: " << s.top() << std::endl;
    std::cout << "Size: " << s.size() << std::endl;

    // Xóa toàn bộ Stack
    std::cout << "\nPop toan bo Stack:\n";

    while (!s.empty())
    {
        std::cout << "Top = " << s.top() << " -> Pop" << std::endl;
        s.pop();
    }

    std::cout << "\nStack rong? "
         << (s.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
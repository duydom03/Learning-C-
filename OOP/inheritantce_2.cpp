#include <iostream>

class Base{
    private:
    protected:
        int m_value {} ;
    public:
        Base(int value) : m_value(value) {}
        void message() {
            std::cout << "Class base : " << std::endl;
        };
};

class Derived : public Base {
    public:
        Derived(int value) : Base(value) {}
        // void message() {
        //     std::cout << "Class derived : " << std::endl;
        // }    
        int getValue() {
                return m_value;
        };
    
};

int main() {

    Base base(10);
    base.message();

    Derived derived(42);
    derived.message();
   

    return 0;
}


#include <iostream>

class Animal{
    public:
        virtual void makeSound()=0;
};

class Dog : public Animal{
    public:
        void makeSound(){
            std::cout<<"Barkkkkk \n";
        }
};

class Cat : public Animal{
    public:
        void makeSound(){
            std::cout<<"Meoooo \n";
        }
};

int main(){
    

    return 0;
}
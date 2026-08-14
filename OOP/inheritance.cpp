#include <iostream>

class Animal {
    protected:
        std::string name ;    
    
    public:
        Animal(const std::string& nameAnimal) : name(nameAnimal){}

        void eat(){
            std::cout<< name <<" eating....\n";
        }
        
        void sleep(){
            std::cout<< name <<" sleeping....\n";
        }
};

class Dog : public Animal {
    public:
        Dog(const std::string& nameDog) : Animal(nameDog){}

        void bark(){
            std::cout<< name <<" barking....\n";
        }
};

class Cat : public Animal {
    public:
        Cat(const std::string& nameCat) : Animal(nameCat){}

        void meow(){
            std::cout<< name <<" meowing....\n";
        }
};

int main() {
    Dog dog("Buddy");
    dog.eat();
    dog.sleep();
    dog.bark();

    Cat cat("Whiskers");
    cat.eat();
    cat.sleep();
    cat.meow();

    return 0;
}
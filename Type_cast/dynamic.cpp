#include <iostream>

class Animal {
public:
    virtual void speak() { std::cout << "Animal sound\n"; } // Bắt buộc phải có virtual
    virtual ~Animal() = default;
    
};

class Dog : public Animal {
public:
    void bark() { std::cout << "Gau gau!\n"; }
};

class Cat : public Animal {
public:
    void meow() { std::cout << "Meo meo!\n"; }
};

void playWithAnimal(Animal* a) {
    // Thử ép kiểu sang Dog*
    Dog* dog = dynamic_cast<Dog*>(a);
    if (dog) {
        dog->bark(); // Chỉ chạy nếu a thực sự là Dog
        return;
    }

    // Thử ép kiểu sang Cat*
    Cat* cat = dynamic_cast<Cat*>(a);
    if (cat) {
        cat->meow(); // Chỉ chạy nếu a thực sự là Cat
        return;
    }

    std::cout << "Animal khac!\n";
}

int main() {
    Dog myDog;
    Cat myCat;

    playWithAnimal(&myDog); // Output: Gau gau!
    playWithAnimal(&myCat); // Output: Meo meo!
}
#include <iostream>
#include <memory>
#include <vector>

class Animal {
public:
    virtual ~Animal() = default;

    virtual void makeSound() const {
        std::cout << "Animal sound" << std::endl;
    }

};

class Dog : public Animal {
    public:
        void makeSound() const override {
            std::cout << "Dog --- Woof!" << std::endl;
        }
};

class Cat : public Animal {
    public:
        void makeSound() const override {
            std::cout << "Cat --- Meow!" << std::endl;
        }
};

class Bird : public Animal {
    public:
        void makeSound() const override {
            std::cout << "Bird --- Chirp!" << std::endl;
        }
};

int main() {
    std::vector<std::unique_ptr<Animal>> animals;

    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());
    animals.push_back(std::make_unique<Bird>());

    for (const auto& animal : animals) {
        animal->makeSound();
    }

    return 0;
}

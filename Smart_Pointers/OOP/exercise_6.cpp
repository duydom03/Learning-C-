#include <iostream>
#include <memory>
#include <vector>

class Animal {
    public:
        virtual ~Animal() {
            std::cout << "Animal destroyed" << std::endl;
        }
        virtual void makeSound() const = 0;
};

class Dog : public Animal {
    public:
        ~Dog() {
            std::cout << "Dog destroyed" << std::endl;
        }

        void makeSound() const override {
            std::cout << "Woof!" << std::endl;
        }
};

class Cat : public Animal {
    public:
        ~Cat() {
            std::cout << "Cat destroyed" << std::endl;
        }

        void makeSound() const override {
            std::cout << "Meow!" << std::endl;
        }
};

// Factory function to create animals based on type
std::unique_ptr<Animal> createAnimal(int type) {
    if (type == 1) {
        return std::make_unique<Dog>();
    } else if (type == 2) {
        return std::make_unique<Cat>();
    } else {
        return nullptr;
    }
}

int main(){
    // Create a vector of unique pointers to Animal
    std::vector<std::unique_ptr<Animal>> animals;

    // Add animals to the vector
    animals.push_back(createAnimal(1));
    animals.push_back(createAnimal(2));
    animals.push_back(createAnimal(1));


    // Iterate through the vector and make each animal sound
    for (const auto& animal : animals) {
        animal->makeSound();
    }

    return 0;
}
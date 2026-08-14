#include <iostream>
#include <memory>

class Car {
    public:
    Car() {
        std::cout << "Car created" << std::endl;
    }
    ~Car() {
        std::cout << "Car destroyed" << std::endl;
    }
};

int main() {
    auto carPtr = std::make_unique<Car>(); // Using unique_ptr to manage the Car object
    // No need to manually delete the Car object; it will be automatically deleted when carPtr goes out of scope.
    auto carPtr2 = std::move(carPtr); // Transfer ownership to carPtr2
    std::cout << "Car is being used" << std::endl;

    if (!carPtr) {
        std::cout << "carPtr is now null after moving ownership." << std::endl;
    }

    std::cout<< "Car is being used from carPtr2" << std::endl;
    
    return 0;
}
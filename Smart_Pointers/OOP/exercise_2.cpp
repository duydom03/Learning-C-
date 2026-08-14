#include <iostream>
#include <memory>
#include <vector>

// This code demonstrates the use of polymorphism and smart pointers in C++20. 
// It defines a base class `Shape` with a pure virtual function `draw()`, and three
// derived classes: `Circle`, `Square`, and `Triangle`, each implementing the `draw()`
// method. The `createShape` function returns a unique pointer to a shape based on the
// provided type. In the `main` function, a shape is created and drawn if it is valid.
class Shape {
    public:
        virtual ~Shape() = default;

        virtual void draw() const = 0; 
};

class Circle : public Shape {
    public:
        void draw() const override {
            std::cout << "Drawing a circle" << std::endl;
        }
};

class Square : public Shape {
    public:
        void draw() const override {
            std::cout << "Drawing a square" << std::endl;
        }
};

class Triangle : public Shape {
    public:
        void draw() const override {
            std::cout << "Drawing a triangle" << std::endl;
        }
};

// Factory function to create shapes based on type
std::unique_ptr<Shape> createShape(int type) {
        switch (type) {
            case 1:
                return std::make_unique<Circle>();
            case 2:
                return std::make_unique<Square>();
            case 3:
                return std::make_unique<Triangle>();
            default:
                return nullptr;
        }
    }


int main(){
    auto shape1 = createShape(3);
    if (shape1) {
        shape1->draw();
    }
    return 0;
}
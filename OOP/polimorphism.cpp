#include <iostream>

class Shape{
    public:
        virtual void draw() {
            std::cout << "Draw shape" << std::endl;
        }
};

class Circle : public Shape {
    public:
        void draw() override {
            std::cout << "Draw circle" << std::endl;
        }
};

class Square : public Shape {
    public:
        void draw() override {
            std::cout << "Draw square" << std::endl;
        }
};

int main() {
    Shape* shape1 = new Circle();
    Shape* shape2 = new Square();

    shape1->draw(); // Output: Draw circle
    shape2->draw(); // Output: Draw square

    delete shape1;
    delete shape2;

    return 0;
}


#include <iostream>
#include <memory>

class Rectangle {
    int length;
    int width;
public:
    Rectangle(int l, int w) : length(l), width(w) {}
    int area() { return length * width; }
};

int main() {
    // Using unique_ptr to manage the Rectangle object
    std::unique_ptr<Rectangle> rectPtr = std::make_unique<Rectangle>(10, 5);
    
    std::cout << "Area of rectangle: " << rectPtr->area() << std::endl;

    // No need to manually delete the Rectangle object; it will be automatically deleted when rectPtr goes out of scope.
    
    std::unique_ptr<Rectangle> RectPtr_2 ;
    RectPtr_2 = std::move(rectPtr); // Transfer ownership to RectPtr_2
    std::cout << "Area of rectangle after moving ownership: " << RectPtr_2->area() << std::endl;

    // After moving ownership, rectPtr is now null
    if (!rectPtr) {
        std::cout << "rectPtr is now null after moving ownership." << std::endl;
    }

    return 0;
}
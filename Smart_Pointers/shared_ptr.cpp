#include <iostream>
#include <memory>

class Rectangle {
    int length;
    int width;
public:
    Rectangle(int l, int w) : length(l), width(w) {}
    int area() { return length * width; }
};

int main(){
    std::shared_ptr<Rectangle> rectPtr(new Rectangle(10, 5));
    std::shared_ptr<Rectangle> rectPtr2 = rectPtr; 
    // Shared ownership of the Rectangle object

    std::cout << "Area of rectangle: " << rectPtr->area() << std::endl;
    std::cout << "Area of rectangle from rectPtr2: " << rectPtr2->area() << std::endl;

    std::cout << "Reference count: " << rectPtr.use_count() << std::endl; 
    // Output the reference count


    return 0;
}
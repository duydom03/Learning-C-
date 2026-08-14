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
    std::weak_ptr<Rectangle> weakRectPtr(rectPtr);
    // weak_ptr does not increase the reference count of the shared_ptr, 
    // so it does not prevent the Rectangle object from being deleted when
    // the last shared_ptr goes out of scope.

    std::cout << "Area of rectangle: " << rectPtr->area() << std::endl;
    std::cout << "Reference count: " << rectPtr.use_count() << std::endl;
    std::cout << "weak_ptr reference count: " << weakRectPtr.use_count() << std::endl;

    return 0;
}
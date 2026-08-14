#include <iostream>
#include "func.h"

int Rectangle::area() {
    return length * width;
}

int Rectangle::perimeter() {
    return 2 * (length + width);
}

void Rectangle::display() {
    std::cout << "Length: " << length << ", Width: " << width << std::endl;
    std::cout << "Area: " << area() << ", Perimeter: " << perimeter() << std::endl;
}
#include <iostream>
#include "calculator.h"

int main() {
    int a = 10, b = 5;

    std::cout << "Addition: " << add(a, b) << std::endl;
    std::cout << "Subtraction: " << subt(a, b) << std::endl;
    std::cout << "Multiplication: " << mult(a, b) << std::endl;

    return 0;
}
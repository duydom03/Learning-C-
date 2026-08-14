#include <iostream>

#define SQUARE(x) ((x) * (x)) // Macro to calculate the square of a number

int main() {
    int a = 5;
    int result = SQUARE(a); // Call the macro

    std::cout << "The square of " << a << " is: " << result << std::endl;

    return 0;
}
#include <iostream>

// Inline function to calculate the square of a number
inline int square(int a) {
    return a * a;
}

int main() {
    int a = 5;
    int result = square(a); // Call the inline function

    std::cout << "The square of " << a << " is: " << result << std::endl;

    return 0;
}
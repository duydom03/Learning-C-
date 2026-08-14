#include <iostream>
#include <functional>

// Macro to calculate the square of a number
#define SQUARE(x) ((x) * (x))

// Inline function to calculate the square of a number
inline int square(int x)
{
    return x * x;
}

// Function object (functor) to calculate the square of a number
struct Square
{
    int operator()(int x) const
    {
        return x * x;
    }
};

// Lambda function to calculate the square of a number
auto lambda = [](int x)
{
    return x * x;
};

// Function pointer to the inline function
int (*fp)(int) = square;

// std::function to wrap the lambda function
std::function<int(int)> func = lambda;

int main()
{
    int a = 5;

    std::cout << "Macro: " << SQUARE(a) << std::endl;
    std::cout << "Inline function: " << square(a) << std::endl;

    Square squareObj;
    std::cout << "Function object: " << squareObj(a) << std::endl;

    std::cout << "Lambda function: " << lambda(a) << std::endl;

    std::cout << "Function pointer: " << fp(a) << std::endl;

    std::cout << "std::function: " << func(a) << std::endl;

    return 0;
}
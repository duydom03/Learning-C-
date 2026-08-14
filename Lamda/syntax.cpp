#include <iostream>

int main() {
    // Lambda function to add two numbers
    auto add = [](int a, int b) {
        return a + b;
    };

    int x = 5;
    int y = 10;
    int result = add(x, y); 

    std::cout << "The sum of " << x << " and " << y << " is: " << result << std::endl;

    // Lambda function to capture the value of x and add it to y
    auto f = [x](int y) {
       
        std::cout<<"The value of x + y is: " << x + y << std::endl;
    };

    f(y); // Call the lambda function to print the value of x

    // Lambda function to capture the value of z (value initial z no changed) 
    int z = 15;
    auto multable =[z]() mutable {
        std::cout<<"The value of z++ after use multable is: " << z+7 << std::endl;
    };
    multable(); // Call the lambda function to print the value of z


    return 0;
}


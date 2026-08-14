#include <iostream>
#include <vector>
#include <algorithm>

int main(){
    std::vector<int> numbers = {5, 2, 9, 1, 5, 6};

    // Find first number divisible by 3 using a lambda function
    auto it =   find_if(numbers.begin(), numbers.end(), [](const int& num) {
        return num % 3 == 0 ;
    });

    if (it != numbers.end()) {
        std::cout << "First number divisible by 3: " << *it << std::endl;
    } else {
        std::cout << "No number divisible by 3 found." << std::endl;
    }
    return 0;
}
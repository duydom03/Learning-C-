#include <iostream>


int main() {
    // declaring and initializing an array of size 5
    int arr[5] = {2, 4, 8, 12, 16};

    // printing array elements
    for (int i = 0; i < 5; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
     // Accessing fourth element
    std::cout << arr[3] << " ";
    
    // Accessing first element
    std::cout << arr[0]<<std::endl;

     // Updating first element
    arr[0] = 90;
    std::cout << arr[0] << std::endl;

    // declaring and initializing a 2D array
    // with 3 rows and 4 columns
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // printing the elements of the 2D array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
#include <iostream>

int main(){

    // This code will cause a memory leak because it continuously allocates memory for an integer without ever freeing it. Each iteration of the loop creates a new integer on the heap, but there is no corresponding delete statement to free that memory. As a result, the program will consume more and more memory over time, which can lead to performance issues or crashes.
    while(1){
        int* ptr = new int;
        
    }
    return 0;
}

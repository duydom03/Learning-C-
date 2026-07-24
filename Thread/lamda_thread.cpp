#include <iostream>
#include <thread>

int main(){
    int n = 6;
    std:: thread t([](int n){
        std::cout<<" Gia tri n : "<<n<<" \n";
    }, n);

    t.join();

    return 0;
}


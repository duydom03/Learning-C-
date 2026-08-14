#include <iostream>
#include <thread>

void functor(int n){
    std::cout<<" Gia tri n : "<<n<<" \n";
}

int main(){
    int n = 6;
    std:: thread t(functor, n);

    t.join();

    return 0;
}


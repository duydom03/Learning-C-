#include<iostream>

int Sum(int a , int b){
    return a + b ;
}


double Sum(double a , double b , double c ){
    return a+b+c;
}

int main(){
    int a ,b ;
    double d , e , f;

    std::cout<<"Sum 2 so int : "<<Sum(5 ,6)<<"\n";
    std::cout<<"Sum 3 so double "<<Sum(1.2 , 3.2 , 5.2);
    return 0;
}
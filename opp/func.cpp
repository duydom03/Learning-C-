#include<iostream>
#include "func.h"
using namespace std ;

int Rectangle ::calArea(){
    return width*hight;
}

int Rectangle :: calPerimeter(){
    return (width+hight)*2;
}

void Rectangle :: disPlay(){
    cout<<"rong : "<< width<<endl;
    cout<<"cao : "<< hight<<endl;
    cout<<"Area : "<< calArea()<<endl;
    cout<<"Perimeter : "<< calPerimeter()<<endl;


}


#ifndef FUNC_H
#define FUNC_H

class Rectangle
{
private:
    int width;
    int hight;

public:
    Rectangle(int w , int h){
        width = w;
        hight = h;
    };

int calArea();
int calPerimeter();
void disPlay();
    
};


#endif
#ifndef FUNC_H
#define FUNC_H

class Rectangle {
private:
    double length;
    double width;

public:
    Rectangle(double l, double w) : length(l), width(w) {}

    int area();
    int perimeter();
    void display();
};

#endif // FUNC_H


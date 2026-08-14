#include <iostream>
#include <fstream>

struct Student
{
    int id;
    std::string name;
    int age;
};


int main()
{
    Student s1{101 , "dom" , 22 };
    std::ofstream file(
        "student.dat",
        std::ios::binary
    );

    file.write(
        reinterpret_cast<char*>(&s1),
        sizeof(s1)
    );

    return 0;
}
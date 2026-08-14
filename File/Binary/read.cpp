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
    Student s ;

    std::ifstream file (
        "student.dat",
        std::ios::binary
    );

    file.read(
        reinterpret_cast<char*>(&s),
        sizeof(s)
    );

    std::cout<< "Student ID : "<< s.id <<"\n";
    std::cout<<"Student age : "<<s.age<<"\n";
    std::cout<<"Student name : "<<s.name<<"\n";
    

    return 0;
}
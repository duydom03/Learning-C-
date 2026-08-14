#include <iostream>
#include "Student.h"

Student::Student(
    int id ,
    const std::string& name,
    int age,
    double score
)
    : id(id),
      name(name),
      age(age),
      score(score)
{    
}

int Student::getID() const
{
    return id;
}

const std::string& Student::getName() const
{
    return name;
}

int Student::getAge() const
{
    return age;
}

double Student::getScore() const
{
    return score;
}

void Student::setName(const std::string& name)
{
    this->name = name;
}

void Student::setAge(int age)
{
    this->age = age;
}

void Student::setScore(double score)
{
    this->score = score;
}

void Student::display() const
{
    std::cout
        <<"ID: "<< id
        <<" | Name: "<< name
        <<" | Age: "<<age
        <<" | Score: "<<score << "\n";
}


#pragma once
#include <string>

class Student
{
    private:
        int id;
        std::string name;
        int age;
        double score;

    public:
        Student(
            int id,
            const std::string& name,
            int age ,
            double score
        );

        int getID() const;
        const std::string& getName() const;
        int getAge() const;
        double getScore() const;

        void setName(const std::string& name);
        void setAge(int age);
        void setScore(double score);

        void display() const;

        
};
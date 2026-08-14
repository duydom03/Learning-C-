#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int age;
    double gpa;

    // Student(const std::string& n, int a, double g) : name(n), age(a), gpa(g) {}
};

int main() {
    std::vector<Student> students = {
        {"Alice", 20, 1.1},
        {"Bob", 22, 3.8},
        {"Charlie", 19, 2.2}
    };

    // [](const Student& a, const Student& b) {return a.gpa > b.gpa;}
    // Sort students by GPA in descending order using a lambda function
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.gpa > b.gpa;
    });

    // Display sorted students
    for (const auto& student : students) {
        std::cout << "Name: " << student.name << ", Age: " << student.age << ", GPA: " << student.gpa << std::endl;
    }

    return 0;
}


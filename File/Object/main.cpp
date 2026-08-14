#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Student
{
private:
    int id;
    std::string name;
    double score;

public:
    Student(int id,
            const std::string& name,
            double score)
        : id(id),
          name(name),
          score(score)
    {
    }

    void save(std::ofstream& file) const
    {
        file << id << ','
             << name << ','
             << score << '\n';
    }
};

int main()
{
    std::vector<Student> students;

    students.emplace_back(1, "Domimic Toreto", 8.5);
    students.emplace_back(2, "Pau Walker", 7.8);
    students.emplace_back(3, "Teran Shaya", 9.2);

    std::ofstream file("students.txt");

    if (!file)
    {
        std::cerr << "Cannot open file\n"; // warning thay vi cout
        return 1;
    }

    for (const auto& student : students)
    {
        student.save(file);
    }

    return 0;
}
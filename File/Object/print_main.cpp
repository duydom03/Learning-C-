#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class Student
{
private:
    int id;
    std::string name;
    double score;

public:
    Student(int id, const std::string& name, double score)
        : id(id), name(name), score(score)
    {
    }

    // đọc 1 đối tượng Student từ chuỗi CSV (1 dòng trong file)
    static Student loadFromLine(const std::string& line)
    {
        std::stringstream ss(line);
        std::string idStr, name, scoreStr;

        // Đọc từng trường dữ liệu phân cách bởi dấu phẩy ','
        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, scoreStr, ',');

        int id = std::stoi(idStr);
        double score = std::stod(scoreStr);

        return Student(id, name, score);
    }

    void print() const
    {
        std::cout << "ID: " << id 
                  << " | Ten: " << name 
                  << " | Diem: " << score << '\n';
    }
};

int main()
{
    
    std::ifstream file("students.txt");

    if (!file)
    {
        std::cerr << "Khong the mo file de doc!\n";
        return 1;
    }

    std::vector<Student> loadedStudents;
    std::string line;

    // Đọc từng dòng cho đến hết file
    while (std::getline(file, line))
    {
        if (!line.empty()) // Bỏ qua dòng trống nếu có
        {
            Student student = Student::loadFromLine(line);
            loadedStudents.push_back(student);
        }
    }

    file.close(); 

    std::cout << "--- DANH SACH HOC SINH DOC TU FILE ---\n";
    for (const auto& student : loadedStudents)
    {
        student.print();
    }

    return 0;
}
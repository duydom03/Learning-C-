#pragma once
#include <vector>
#include "models.h"
#include "database.h"

class StudentManager {
public:
    explicit StudentManager(Database& db);

    // Create a new student. class_id may be -1/0 for unassigned.
    // Returns the new student id, or -1 on failure.
    int addStudent(const std::string& name, const std::string& email, int class_id);

    // Read all students.
    std::vector<Student> listAll();

    // Read students of a specific class.
    std::vector<Student> listByClass(int class_id);

    // Update a student by id. Returns true on success.
    bool updateStudent(int id, const std::string& name, const std::string& email, int class_id);

    // Delete a student by id. Returns true on success.
    bool deleteStudent(int id);

    // Assign / reassign a student to a class. Use class_id = -1/0 to unassign.
    // Returns true on success.
    bool assignToClass(int student_id, int class_id);

    // Count students per class. Returns a vector of (class_id, class_name, count).
    struct ClassCount {
        int class_id;
        std::string class_name;
        int count;
    };
    std::vector<ClassCount> countByClass();

    // Search students by name (partial match, case-insensitive).
    std::vector<Student> searchByName(const std::string& keyword);

    // List classes that have zero students.
    std::vector<Class> classesWithZeroStudents();

private:
    Database& db_;
};
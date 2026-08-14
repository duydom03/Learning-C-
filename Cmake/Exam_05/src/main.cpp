#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include "database.h"
#include "class_manager.h"
#include "student_manager.h"

// Read a line from stdin, trimming trailing whitespace.
static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// Read an integer from stdin. Returns true on success.
static bool readInt(const std::string& prompt, int& out) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    try {
        out = std::stoi(line);
        return true;
    } catch (...) {
        return false;
    }
}

static void printClasses(const std::vector<Class>& classes) {
    if (classes.empty()) {
        std::cout << "  (no classes)\n";
        return;
    }
    std::cout << "  ID    | Name    | Room\n";
    std::cout << "  ------+---------+------\n";
    for (const auto& c : classes) {
        std::cout << "  " << c.id << "    | " << c.name << " | " << c.room << "\n";
    }
}

static void printStudents(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "  (no students)\n";
        return;
    }
    std::cout << "  ID    | Name            | Email             | Class\n";
    std::cout << "  ------+-----------------+-------------------+-------\n";
    for (const auto& s : students) {
        std::cout << "  " << s.id << "    | " << s.name << " | " << s.email
                  << " | " << (s.class_id > 0 ? std::to_string(s.class_id) : std::string("-")) << "\n";
    }
}

static void classMenu(ClassManager& cm) {
    while (true) {
        std::cout << "\n--- Class Management ---\n";
        std::cout << "1. Add class\n";
        std::cout << "2. List classes\n";
        std::cout << "3. Update class\n";
        std::cout << "4. Delete class\n";
        std::cout << "0. Back\n";
        int choice = 0;
        if (!readInt("> ", choice)) continue;
        if (choice == 0) break;
        switch (choice) {
            case 1: {
                std::string name = readLine("Name: ");
                std::string room = readLine("Room: ");
                int id = cm.addClass(name, room);
                if (id > 0) std::cout << "Added class id=" << id << "\n";
                else std::cout << "Failed to add class.\n";
                break;
            }
            case 2: {
                printClasses(cm.listClasses());
                break;
            }
            case 3: {
                int id = 0;
                if (!readInt("Class id: ", id)) { std::cout << "Invalid id.\n"; break; }
                std::string name = readLine("New name: ");
                std::string room = readLine("New room: ");
                if (cm.updateClass(id, name, room)) std::cout << "Updated.\n";
                else std::cout << "Update failed (id not found or error).\n";
                break;
            }
            case 4: {
                int id = 0;
                if (!readInt("Class id: ", id)) { std::cout << "Invalid id.\n"; break; }
                if (cm.deleteClass(id)) std::cout << "Deleted (students unassigned).\n";
                else std::cout << "Delete failed (id not found or error).\n";
                break;
            }
            default:
                std::cout << "Invalid option.\n";
        }
    }
}

static void studentMenu(StudentManager& sm) {
    while (true) {
        std::cout << "\n--- Student Management ---\n";
        std::cout << "1. Add student\n";
        std::cout << "2. List all students\n";
        std::cout << "3. List students by class\n";
        std::cout << "4. Update student\n";
        std::cout << "5. Delete student\n";
        std::cout << "6. Assign student to class\n";
        std::cout << "0. Back\n";
        int choice = 0;
        if (!readInt("> ", choice)) continue;
        if (choice == 0) break;
        switch (choice) {
            case 1: {
                std::string name = readLine("Name: ");
                std::string email = readLine("Email: ");
                int classId = 0;
                readInt("Class id (0 for none): ", classId);
                int id = sm.addStudent(name, email, classId);
                if (id > 0) std::cout << "Added student id=" << id << "\n";
                else std::cout << "Failed to add student.\n";
                break;
            }
            case 2: {
                printStudents(sm.listAll());
                break;
            }
            case 3: {
                int classId = 0;
                if (!readInt("Class id: ", classId)) { std::cout << "Invalid id.\n"; break; }
                printStudents(sm.listByClass(classId));
                break;
            }
            case 4: {
                int id = 0;
                if (!readInt("Student id: ", id)) { std::cout << "Invalid id.\n"; break; }
                std::string name = readLine("New name: ");
                std::string email = readLine("New email: ");
                int classId = 0;
                readInt("New class id (0 for none): ", classId);
                if (sm.updateStudent(id, name, email, classId)) std::cout << "Updated.\n";
                else std::cout << "Update failed (id not found or error).\n";
                break;
            }
            case 5: {
                int id = 0;
                if (!readInt("Student id: ", id)) { std::cout << "Invalid id.\n"; break; }
                if (sm.deleteStudent(id)) std::cout << "Deleted.\n";
                else std::cout << "Delete failed (id not found or error).\n";
                break;
            }
            case 6: {
                int studentId = 0, classId = 0;
                if (!readInt("Student id: ", studentId)) { std::cout << "Invalid id.\n"; break; }
                if (!readInt("Class id (0 to unassign): ", classId)) { std::cout << "Invalid id.\n"; break; }
                if (sm.assignToClass(studentId, classId)) std::cout << "Assigned.\n";
                else std::cout << "Assign failed (id not found or error).\n";
                break;
            }
            default:
                std::cout << "Invalid option.\n";
        }
    }
}

static void statsMenu(StudentManager& sm) {
    std::cout << "\n--- Statistics: Students per class ---\n";
    auto counts = sm.countByClass();
    if (counts.empty()) {
        std::cout << "  (no classes)\n";
        return;
    }
    std::cout << "  Class   | Name    | Students\n";
    std::cout << "  --------+---------+----------\n";
    for (const auto& cc : counts) {
        std::cout << "  " << cc.class_id << "      | " << cc.class_name << " | " << cc.count << "\n";
    }
}

static void searchMenu(StudentManager& sm) {
    std::cout << "\n--- Search ---\n";
    std::cout << "1. Search students by name\n";
    std::cout << "2. List classes with zero students\n";
    std::cout << "0. Back\n";
    int choice = 0;
    if (!readInt("> ", choice)) return;
    switch (choice) {
        case 1: {
            std::string keyword = readLine("Name keyword: ");
            printStudents(sm.searchByName(keyword));
            break;
        }
        case 2: {
            auto classes = sm.classesWithZeroStudents();
            if (classes.empty()) std::cout << "  (all classes have students)\n";
            else printClasses(classes);
            break;
        }
        case 0:
            break;
        default:
            std::cout << "Invalid option.\n";
    }
}

int main() {
    auto db = std::make_unique<Database>("school.db");
    if (!db->raw()) {
        std::cerr << "Failed to open database. Exiting.\n";
        return 1;
    }

    ClassManager cm(*db);
    StudentManager sm(*db);

    while (true) {
        std::cout << "\n===== CLASS & STUDENT MANAGEMENT =====\n";
        std::cout << "1. Class management\n";
        std::cout << "2. Student management\n";
        std::cout << "3. Statistics\n";
        std::cout << "4. Search\n";
        std::cout << "0. Exit\n";
        int choice = 0;
        if (!readInt("> ", choice)) continue;
        if (choice == 0) break;
        switch (choice) {
            case 1: classMenu(cm); break;
            case 2: studentMenu(sm); break;
            case 3: statsMenu(sm); break;
            case 4: searchMenu(sm); break;
            default: std::cout << "Invalid option.\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
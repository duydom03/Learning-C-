# Exam_05 — Class & Student Management System (C++ + SQLite)

A console-based C++ application to manage **classes** and **students**, with data persisted in a SQLite database.

## Tech Stack
- C++17
- CMake ≥ 3.20
- SQLite (system-installed via MSYS2 UCRT64)
- OOP, RAII, smart pointers

## Project Structure
```
Exam_05/
├── CMakeLists.txt
├── schema.sql          # Table definitions
├── seed.sql            # Sample data (3 classes, 12 students)
├── README.md
├── include/
│   ├── models.h
│   ├── database.h
│   ├── class_manager.h
│   └── student_manager.h
└── src/
    ├── main.cpp
    ├── database.cpp
    ├── class_manager.cpp
    └── student_manager.cpp
```

## Build & Run (Bash)

```bash
# From the Exam_05 directory
cmake -B build && cmake --build build

# Run (schema.sql & seed.sql are loaded automatically on first run)
./build/main.exe
```

> The database file `school.db` is created in the working directory. Delete it to reset to seed data.

## Features

### Class Management
- Add / list / update / delete classes
- Deleting a class unassigns its students (FK `ON DELETE SET NULL`)

### Student Management
- Add / list (all or by class) / update / delete students
- Assign / reassign a student to a class

### Statistics & Search
- Count students per class
- Search students by name (case-insensitive, partial match)
- List classes with zero students

## Database Schema
```sql
classes(id PK, name UNIQUE, room)
students(id PK, name, email UNIQUE, class_id FK -> classes.id ON DELETE SET NULL)
```

## Self-Debugging Log
| Module | Bug found | Fix |
|--------|-----------|-----|
| Database | (none on first compile) | — |
| ClassManager | (none) | — |
| StudentManager | (none) | — |
| main | (none) | — |

> Build verified with `-Wall -Wextra`, zero warnings.
# Prompt: Class & Student Management System (C++ + SQLite)

## 1. Role & Context

You are a senior C++ software engineer. Your task is to build a **Class & Student Management System** — a console-based C++ application that persists data in a **SQLite** database.

**Target environment (already configured on this machine):**
- OS: Windows 11
- Toolchain: MSYS2 UCRT64 — `g++ 16.1.0`, CMake ≥ 3.20
- SQLite is **pre-installed** and available system-wide:
  - Header: `C:\msys64\ucrt64\include\sqlite3.h`
  - Libraries: `C:\msys64\ucrt64\lib\libsqlite3.a`, `libsqlite3.dll.a`
  - CLI: `C:\msys64\ucrt64\bin\sqlite3.exe`
- C++ standard: **C++17**
- **Shell: use Bash** (MSYS2 bash or Git Bash). All commands in this prompt use Bash syntax (forward slashes, `./` prefix for executables, `&&` chaining). Do **not** use PowerShell or cmd.exe syntax.

You do **not** need to install anything. Link SQLite via CMake `find_package` or direct library path.

---

## 2. Problem Statement

Build a console application to manage **classes** and **students** at a small school.

**Scale:**
- 2–4 classes
- 10–15 students per class

**Data must be persisted** in a SQLite database file (`school.db`) so that records survive across application restarts.

---

## 3. Tech Stack & Constraints

| Item | Requirement |
|------|-------------|
| Language | C++17 |
| Build system | CMake ≥ 3.20 |
| Database | SQLite (via `sqlite3.h` + `libsqlite3`) |
| Paradigm | Object-Oriented Programming (classes, encapsulation, inheritance where appropriate) |
| Header guards | `#pragma once` |
| File organization | Separate `include/` (headers) and `src/` (implementations) |
| Naming | `PascalCase` for classes, `camelCase` for methods/variables (match existing codebase style) |
| Memory | Prefer `std::unique_ptr` / `std::shared_ptr` over raw `new`/`delete` |
| Error handling | Check every SQLite return code; print meaningful error messages |

**CMakeLists.txt must:**
- Set `CMAKE_CXX_STANDARD 17`
- Create a static or shared library for the core logic
- Create an executable `main` that links the core library **and** `sqlite3`
- Use `target_include_directories` pointing to `include/` (not `include/subfolder`)

---

## 4. Database Schema

Create a file `schema.sql` with the following design:

```sql
-- classes table
CREATE TABLE IF NOT EXISTS classes (
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    name  TEXT    NOT NULL UNIQUE,
    room  TEXT
);

-- students table
CREATE TABLE IF NOT EXISTS students (
    id        INTEGER PRIMARY KEY AUTOINCREMENT,
    name      TEXT    NOT NULL,
    email     TEXT    UNIQUE,
    class_id  INTEGER,
    FOREIGN KEY (class_id) REFERENCES classes(id) ON DELETE SET NULL
);
```

**Relationship:** One class → many students (1‑N).

Also create `seed.sql` with sample data (see Section 9).

---

## 5. Features (Full CRUD + Statistics)

### 5.1 Class Management
- **Create** — add a new class (name, room)
- **Read** — list all classes
- **Update** — edit class name/room by id
- **Delete** — remove a class (students in it get `class_id = NULL` via FK `ON DELETE SET NULL`)

### 5.2 Student Management
- **Create** — add a student (name, email, class_id)
- **Read** — list all students / list students of a specific class
- **Update** — edit student info by id
- **Delete** — remove a student by id
- **Assign** — assign / reassign a student to a class

### 5.3 Statistics & Search
- Count students per class
- Search students by name (partial match, case-insensitive)
- List classes with zero students

### 5.4 Console Menu
Provide a text-based menu loop in `main.cpp`:
```
===== CLASS & STUDENT MANAGEMENT =====
1. Class management
2. Student management
3. Statistics
4. Search
0. Exit
> 
```
Sub-menus for each section with CRUD options.

---

## 6. Project Structure

```
Exam_05/
├── CMakeLists.txt
├── schema.sql
├── seed.sql
├── README.md
├── include/
│   ├── database.h
│   ├── class_manager.h
│   ├── student_manager.h
│   └── models.h
└── src/
    ├── main.cpp
    ├── database.cpp
    ├── class_manager.cpp
    └── student_manager.cpp
```

---

## 7. Suggested OOP Design

### `models.h`
```cpp
#pragma once
#include <string>

struct Class {
    int id;
    std::string name;
    std::string room;
};

struct Student {
    int id;
    std::string name;
    std::string email;
    int class_id;   // -1 or 0 if unassigned
};
```

### `Database` (RAII wrapper around `sqlite3*`)
- Constructor: open `school.db`, execute `schema.sql` if tables don't exist
- Destructor: close connection
- Provide `execute()`, `prepare()`, `query()` helper methods
- **Non-copyable**, movable

### `ClassManager`
- Methods: `addClass`, `listClasses`, `updateClass`, `deleteClass`
- Holds a reference/pointer to `Database`

### `StudentManager`
- Methods: `addStudent`, `listAll`, `listByClass`, `updateStudent`, `deleteStudent`, `assignToClass`, `countByClass`, `searchByName`
- Holds a reference/pointer to `Database`

### `main.cpp`
- Instantiate `Database`, `ClassManager`, `StudentManager`
- Run the menu loop

---

## 8. Self-Debugging Workflow (MANDATORY)

> **This is a hard requirement. Do NOT skip.**

You must follow this incremental, self-verifying workflow for **every module** before moving to the next:

```
For each module (Database → ClassManager → StudentManager → main):
  1. Write the code for that module.
  2. Compile immediately (Bash):
       cmake -B build && cmake --build build
  3. Run a quick smoke test:
       ./build/main.exe
  4. Identify ANY bug (compile error, runtime error, logic error, SQL error, memory leak).
  5. Fix the bug RIGHT NOW.
  6. Re-compile and re-test (repeat steps 2–3).
  7. ONLY when the module is bug-free, proceed to the next module.
```

**Rules:**
- ❌ Do NOT write the entire project at once and then debug.
- ❌ Do NOT move to the next step if the current step has unresolved bugs.
- ✅ If a bug is found, stop, fix it, verify the fix, then continue.
- ✅ Print a short "✓ Module X passed" confirmation before moving on.
- ✅ If you encounter a bug you cannot resolve after 2 attempts, **stop and ask the user** for guidance — do not guess blindly.

---

## 9. Sample Data (`seed.sql`)

Insert 3 classes and ~12 students total:

```sql
INSERT INTO classes (name, room) VALUES
    ('CSE101', 'Room A1'),
    ('CSE102', 'Room B2'),
    ('CSE103', 'Room C3');

INSERT INTO students (name, email, class_id) VALUES
    ('Alice Nguyen',   'alice@school.com',   1),
    ('Bob Tran',       'bob@school.com',     1),
    ('Charlie Le',     'charlie@school.com', 1),
    ('Diana Pham',     'diana@school.com',   1),
    ('Ethan Vo',       'ethan@school.com',   2),
    ('Fiona Hoang',    'fiona@school.com',   2),
    ('George Dang',    'george@school.com',  2),
    ('Hannah Bui',     'hannah@school.com',  2),
    ('Ian Truong',     'ian@school.com',     3),
    ('Jane Lam',       'jane@school.com',    3),
    ('Kevin Mac',      'kevin@school.com',   3),
    ('Lily Ngo',       'lily@school.com',    3);
```

The application should load this seed data on first run (when the database is empty).

---

## 10. Acceptance Criteria

The project is complete when **all** of the following are true:

1. ✅ `cmake -B build && cmake --build build` succeeds with **zero warnings** (use `-Wall -Wextra`).
2. ✅ Running `./build/main.exe` (from Bash) opens the console menu.
3. ✅ All CRUD operations work and changes persist in `school.db` across restarts.
4. ✅ Statistics (count per class) and search (by name) return correct results.
5. ✅ Deleting a class sets affected students' `class_id` to NULL (not crash).
6. ✅ No memory leaks (verify with smart pointers, no raw `new`/`delete`).
7. ✅ Every SQLite call checks its return code and handles errors gracefully.
8. ✅ The self-debugging workflow (Section 8) was followed — each module verified before the next.

---

## 11. Deliverables

1. Complete source tree per Section 6.
2. `CMakeLists.txt` that links SQLite correctly.
3. `schema.sql` and `seed.sql`.
4. `README.md` with build & run instructions.
5. A short log of the self-debugging process (which bugs were found & fixed per module).

---

**Start now. Begin with the `Database` module. Compile, test, fix bugs, then proceed.**
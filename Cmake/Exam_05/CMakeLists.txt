cmake_minimum_required(VERSION 3.20)
project(Exam_05 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warnings as required by acceptance criteria
add_compile_options(-Wall -Wextra)

# 1. Core library: database + managers
add_library(core STATIC
    src/database.cpp
    src/class_manager.cpp
    src/student_manager.cpp
)

target_include_directories(core
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link SQLite (available system-wide in MSYS2 UCRT64)
target_link_libraries(core PUBLIC sqlite3)

# 2. Executable
add_executable(main
    src/main.cpp
)

target_link_libraries(main
PRIVATE
    core
)
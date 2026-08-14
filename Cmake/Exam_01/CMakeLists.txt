cmake_minimum_required(VERSION 3.20)

project(Exam_01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(calculator
    src/main.cpp
    src/calculator.cpp
)

target_include_directories(calculator
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
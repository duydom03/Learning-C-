cmake_minimum_required(VERSION 3.20)
project(Exam_02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# using static library 
# Create a static library from calculator.cpp
add_library(calculator STATIC
    src/calculator.cpp
)

# Specify include directory for the library target
target_include_directories(calculator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create the executable from main.cpp
add_executable(main
    src/main.cpp
)

# Link the static library to the executable
target_link_libraries(main PRIVATE calculator)

# The executable also inherits the include path from calculator's PUBLIC interface


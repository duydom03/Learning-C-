cmake_minimum_required(VERSION 3.20)
project(Exam_03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if use #include <thread> , you must link in cmakelist
#find_package(Threads REQUIRED)

# same function

# target_compile_features(main
#     PRIVATE
#         cxx_std_17
# )

# 1. Tạo thư viện 'math' include/math/calculator.h
add_library(math STATIC
    src/calculator.cpp
)

# 2. SỬA TẠI ĐÂY: Chỉ trỏ tới 'include', không trỏ vào 'include/math'
target_include_directories(math 
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 3. Tạo file thực thi 'main'
add_executable(main
    src/main.cpp
)

# 4. Link thư viện
target_link_libraries(main 
PRIVATE 
    math
)
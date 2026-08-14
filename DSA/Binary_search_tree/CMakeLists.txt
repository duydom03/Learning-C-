cmake_minimum_required(VERSION 3.20)

# Tên dự án
project(Binary_search_tree LANGUAGES CXX)

# Thiết lập chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# 1. TẠO THƯ VIỆN (Library) từ mã nguồn trong src/
# Dùng STATIC để tạo thư viện tĩnh (.lib trên Windows, .a trên Linux/macOS)
# (Nếu muốn dùng thư viện động, thay STATIC thành SHARED)
add_library(bst_lib STATIC
    src/BinarySearchTree.cpp
)

# 2. SỬA TẠI ĐÂY: Chỉ trỏ tới 'include', không trỏ vào 'include/math'
target_include_directories(bst_lib 
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



# 2. TẠO FILE THỰC THI (Executable) chỉ chứa main.cpp
add_executable(main_app
    src/main.cpp
)

# 3. LIÊN KẾT (Link) thư viện bst_lib vào file thực thi main_app
target_link_libraries(main_app PRIVATE bst_lib)
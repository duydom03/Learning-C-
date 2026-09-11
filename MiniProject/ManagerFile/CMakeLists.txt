cmake_minimum_required(VERSION 3.16)
project(ManagerFileWorkspace VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets Quick Sql Concurrent)

add_subdirectory(server)
add_subdirectory(client)

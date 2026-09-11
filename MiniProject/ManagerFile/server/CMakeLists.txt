cmake_minimum_required(VERSION 3.16)
project(LicenseServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

add_executable(appLicenseServer
    main.cpp
    licenseserver.h
    licenseserver.cpp
)

target_link_libraries(appLicenseServer
    PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Sql
)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)



add_library(FileManagerLib STATIC
    filemodel.cpp
    filemodel.h
    foldermodel.cpp
    foldermodel.h
    licensemanager.cpp
    licensemanager.h
)

add_executable(appFileManager
    main.cpp
)

target_link_libraries(appFileManager
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Qml
    PRIVATE Qt6::QuickControls2
)


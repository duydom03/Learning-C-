cmake_minimum_required(VERSION 3.16)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appManagerFile
    main.cpp
    core/appcore.h core/appcore.cpp
    core/licenseinfo.h core/licenseinfo.cpp
    core/licenseclient.h core/licenseclient.cpp
    core/filemodel.h core/filemodel.cpp
    core/foldermodel.h core/foldermodel.cpp
    core/threadmodel.h core/threadmodel.cpp
    core/uploadtask.h core/uploadtask.cpp
    core/dbmanager.h core/dbmanager.cpp
    core/fileiconprovider.h core/fileiconprovider.cpp
)

qt_add_qml_module(appManagerFile
    URI ManagerFile
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/LicenseView.qml
        qml/MainView.qml
        qml/components/Toolbar.qml
        qml/components/NavigationPane.qml
        qml/components/ContentArea.qml
        qml/components/FileItemDelegate.qml
        qml/components/ProgressOverlay.qml
        qml/components/LogViewer.qml
)

target_link_libraries(appManagerFile
    PRIVATE
        Qt6::Quick
        Qt6::Network
        Qt6::Sql
        Qt6::Concurrent
        Qt6::Widgets
)

set_target_properties(appManagerFile PROPERTIES
    WIN32_EXECUTABLE TRUE
)

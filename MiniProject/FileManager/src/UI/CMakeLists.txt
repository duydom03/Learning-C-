add_subdirectory(Components)
add_subdirectory(Styles)
add_subdirectory(Icons)

qt_add_qml_module(appFileManager
    URI FileManager
    QML_FILES
        main.qml
)

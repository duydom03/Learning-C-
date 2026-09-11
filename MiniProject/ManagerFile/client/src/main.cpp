#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QDebug>

#include "core/appcore.h"
#include "core/fileiconprovider.h"

int main(int argc, char *argv[])
{
    // Use QApplication instead of QGuiApplication for native QFileDialog support
    QApplication app(argc, argv);
    app.setApplicationName("FileManagerDrive");
    app.setOrganizationName("Antigravity");

    QQmlApplicationEngine engine;

    // Register FileIconProvider for image://fileicon/... URLs in QML
    engine.addImageProvider("fileicon", new FileIconProvider);

    // Initialize AppCore hub
    AppCore appCore;

    // Expose context properties to QML as specified in the Implementation Plan
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("appCore",      &appCore);
    context->setContextProperty("licenseInfo",  appCore.licenseInfo());
    context->setContextProperty("fileModel",    appCore.fileModel());
    context->setContextProperty("folderModel",  appCore.folderModel());
    context->setContextProperty("threadModel",  appCore.threadModel());

    // Connect objectCreationFailed signal
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Load Main.qml from QML module
    engine.loadFromModule("ManagerFile", "Main");

    qInfo() << "==============================================";
    qInfo() << " File Manager Client started successfully!";
    qInfo() << " Machine ID:" << appCore.licenseInfo();
    qInfo() << "==============================================";

    return app.exec();
}

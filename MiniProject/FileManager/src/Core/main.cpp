#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QQuickStyle>

#include "licensemanager.h"
#include "filemodel.h"
#include "foldermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("FileManager");
    app.setApplicationVersion("2.4.1");
    app.setOrganizationName("FM Corp");
    app.setOrganizationDomain("filemanager.io");
    app.setWindowIcon(QIcon(":/resources/appicon.png"));

    // Use Material style
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    // Register C++ objects as context properties
    LicenseManager licenseManager;
    FileModel fileModel;
    FolderModel folderModel;

    engine.rootContext()->setContextProperty("licenseManager", &licenseManager);
    engine.rootContext()->setContextProperty("fileModel", &fileModel);
    engine.rootContext()->setContextProperty("folderModel", &folderModel);

    // Register QML types
    qmlRegisterType<LicenseManager>("FileManager.Backend", 1, 0, "LicenseManager");
    qmlRegisterType<FileModel>("FileManager.Backend", 1, 0, "FileModel");
    qmlRegisterType<FolderModel>("FileManager.Backend", 1, 0, "FolderModel");

    const QUrl url(u"qrc:/main.qml"_qs);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );

    engine.load(url);

    return app.exec();
}

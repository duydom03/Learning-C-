#include <QCoreApplication>
#include "licenseserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("LicenseServer");
    QCoreApplication::setApplicationVersion("0.1");

    LicenseServer server;
    if (!server.startServer(9099)) {
        qCritical() << "Failed to start License Server. Exiting.";
        return -1;
    }

    return QCoreApplication::exec();
}

#ifndef LICENSESERVER_H
#define LICENSESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

class LicenseServer : public QObject
{
    Q_OBJECT
public:
    explicit LicenseServer(QObject *parent = nullptr);
    ~LicenseServer();

    bool startServer(quint16 port = 9099);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *m_tcpServer;
    QSqlDatabase m_db;

    bool initDatabase();
    void processRequest(QTcpSocket *socket, const QByteArray &data);
};

#endif // LICENSESERVER_H

#ifndef LICENSECLIENT_H
#define LICENSECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class LicenseInfo;
class DbManager;

class LicenseClient : public QObject
{
    Q_OBJECT
public:
    explicit LicenseClient(LicenseInfo *info, DbManager *db, QObject *parent = nullptr);

    QString machineId() const;

public slots:
    void verifyKey(const QString &licenseKey);

signals:
    void verificationComplete(bool success);

private slots:
    void onConnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpSocket *m_socket;
    LicenseInfo *m_licenseInfo;
    DbManager *m_dbManager;
    QString m_machineId;
    QString m_pendingKey;
};

#endif // LICENSECLIENT_H

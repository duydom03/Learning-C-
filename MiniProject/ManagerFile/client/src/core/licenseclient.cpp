#include "licenseclient.h"
#include "licenseinfo.h"
#include "dbmanager.h"

#include <QSysInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

LicenseClient::LicenseClient(LicenseInfo *info, DbManager *db, QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_licenseInfo(info)
    , m_dbManager(db)
{
    // Retrieve unique machine identifier
    QByteArray rawId = QSysInfo::machineUniqueId();
    m_machineId = rawId.isEmpty()
                      ? QSysInfo::machineHostName()
                      : QString::fromLatin1(rawId.toHex());

    connect(m_socket, &QTcpSocket::connected,
            this, &LicenseClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead,
            this, &LicenseClient::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred,
            this, &LicenseClient::onErrorOccurred);
}

QString LicenseClient::machineId() const
{
    return m_machineId;
}

void LicenseClient::verifyKey(const QString &licenseKey)
{
    m_pendingKey = licenseKey;
    m_licenseInfo->setError("Đang kết nối đến License Server...");

    // Disconnect any existing connection first
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->abort();
    }

    m_socket->connectToHost("127.0.0.1", 9099);
}

void LicenseClient::onConnected()
{
    qInfo() << "[Client] Connected to License Server";
    m_licenseInfo->setError("Đã kết nối. Đang kiểm tra mã key...");

    QJsonObject request;
    request["machineId"]  = m_machineId;
    request["licenseKey"] = m_pendingKey;

    QByteArray data = QJsonDocument(request).toJson(QJsonDocument::Compact);
    m_socket->write(data);
    m_socket->flush();
}

void LicenseClient::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isObject()) {
        m_licenseInfo->setError("Phản hồi không hợp lệ từ server");
        m_dbManager->logLicenseAttempt(m_pendingKey, m_machineId, "Error", 0);
        emit verificationComplete(false);
        return;
    }

    QJsonObject response = doc.object();
    QString status   = response.value("status").toString();
    QString username = response.value("username").toString();
    QString mid      = response.value("machineId").toString();
    int days         = response.value("days").toInt();
    QString plan     = response.value("plan").toString();
    QString message  = response.value("message").toString();

    if (status == "valid") {
        m_licenseInfo->setValid(true, username, mid.isEmpty() ? m_machineId : mid, plan, days, message);
        m_dbManager->logLicenseAttempt(m_pendingKey, m_machineId, "Success (" + username + ")", days);
        qInfo() << "[Client] License VALID for User:" << username << plan << days << "days";
        emit verificationComplete(true);
    } else {
        m_licenseInfo->setError(message.isEmpty() ? "Product Key không hợp lệ" : message);
        m_dbManager->logLicenseAttempt(m_pendingKey, m_machineId, "Failed", 0);
        qWarning() << "[Client] License INVALID:" << message;
        emit verificationComplete(false);
    }

    m_socket->disconnectFromHost();
}

void LicenseClient::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    QString errorMsg = QString("Lỗi kết nối Server: %1").arg(m_socket->errorString());
    m_licenseInfo->setError(errorMsg);
    m_dbManager->logLicenseAttempt(m_pendingKey, m_machineId, "Connection Error", 0);
    qWarning() << "[Client]" << errorMsg;
    emit verificationComplete(false);
}

#include "licenseserver.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

LicenseServer::LicenseServer(QObject *parent)
    : QObject(parent), m_tcpServer(new QTcpServer(this)) {
  connect(m_tcpServer, &QTcpServer::newConnection, this,
          &LicenseServer::onNewConnection);
}

LicenseServer::~LicenseServer() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}

bool LicenseServer::initDatabase() {
  m_db = QSqlDatabase::addDatabase("QSQLITE", "server_db_connection");
  m_db.setDatabaseName("server_license.db");

  if (!m_db.open()) {
    qCritical() << "[Server DB] Failed to open database:"
                << m_db.lastError().text();
    return false;
  }

  QSqlQuery query(m_db);
  bool ok = query.exec("CREATE TABLE IF NOT EXISTS licenses ("
                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "  license_key TEXT UNIQUE NOT NULL,"
                       "  username TEXT NOT NULL,"
                       "  machine_id TEXT DEFAULT '',"
                       "  days_remaining INTEGER NOT NULL,"
                       "  plan_name TEXT NOT NULL,"
                       "  is_active INTEGER DEFAULT 1"
                       ")");

  if (!ok) {
    qCritical() << "[Server DB] Failed to create licenses table:"
                << query.lastError().text();
    return false;
  }

  // Insert pre-populated sample product keys if database is empty
  query.exec("SELECT COUNT(*) FROM licenses");
  if (query.next() && query.value(0).toInt() == 0) {
    qInfo() << "[Server DB] Populating default sample keys...";

    QSqlQuery insertQuery(m_db);
    insertQuery.prepare("INSERT INTO licenses (license_key, username, "
                        "days_remaining, plan_name, is_active) "
                        "VALUES (:key, :user, :days, :plan, 1)");

    // Key 1: Primary requested key format ADFE-3JCS-AKS1-HN24
    insertQuery.bindValue(":key", "ADFE-3JCS-AKS1-HN24");
    insertQuery.bindValue(":user", "Duy Dom");
    insertQuery.bindValue(":days", 365);
    insertQuery.bindValue(":plan", "Premium Gold (365 Ngày)");
    insertQuery.exec();

    // Key 2: Pro Business Key
    insertQuery.bindValue(":key", "PRO-USER-2026-KEY1");
    insertQuery.bindValue(":user", "Messi ");
    insertQuery.bindValue(":days", 180);
    insertQuery.bindValue(":plan", "Pro Business (180 Ngày)");
    insertQuery.exec();

    // Key 3: Trial Key
    insertQuery.bindValue(":key", "TRIAL-8888-9999-7DAY");
    insertQuery.bindValue(":user", "ZengShiWey");
    insertQuery.bindValue(":days", 7);
    insertQuery.bindValue(":plan", "Trial Demo (7 Ngày)");
    insertQuery.exec();
  }

  qInfo() << "[Server DB] SQLite database server_license.db ready!";
  return true;
}

bool LicenseServer::startServer(quint16 port) {
  if (!initDatabase()) {
    qCritical() << "[Server] Database initialization failed. Aborting.";
    return false;
  }

  if (!m_tcpServer->listen(QHostAddress::Any, port)) {
    qCritical() << "[Server] Failed to start on port" << port << ":"
                << m_tcpServer->errorString();
    return false;
  }

  qInfo() << "==========================================================";
  qInfo() << " License Server listening on port" << port;
  qInfo() << " Active Product Keys in SQLite database (server_license.db):";

  QSqlQuery q("SELECT license_key, username, days_remaining, plan_name FROM "
              "licenses WHERE is_active=1",
              m_db);
  while (q.next()) {
    qInfo() << "   KEY:" << q.value(0).toString()
            << "| User:" << q.value(1).toString()
            << "| Days:" << q.value(2).toInt()
            << "| Plan:" << q.value(3).toString();
  }
  qInfo() << "==========================================================";
  return true;
}

void LicenseServer::onNewConnection() {
  while (m_tcpServer->hasPendingConnections()) {
    QTcpSocket *socket = m_tcpServer->nextPendingConnection();
    qInfo() << "[Server] New client connected from"
            << socket->peerAddress().toString() << ":" << socket->peerPort();

    connect(socket, &QTcpSocket::readyRead, this, &LicenseServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this,
            &LicenseServer::onClientDisconnected);
  }
}

void LicenseServer::onReadyRead() {
  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  if (!socket)
    return;

  QByteArray data = socket->readAll();
  qInfo() << "[Server] Received TCP payload:" << data;
  processRequest(socket, data);
}

void LicenseServer::onClientDisconnected() {
  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  if (socket) {
    qInfo() << "[Server] Client disconnected:"
            << socket->peerAddress().toString();
    socket->deleteLater();
  }
}

void LicenseServer::processRequest(QTcpSocket *socket, const QByteArray &data) {
  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (doc.isNull() || !doc.isObject()) {
    QJsonObject errorResp;
    errorResp["status"] = "error";
    errorResp["message"] = "Dữ liệu JSON không hợp lệ";
    socket->write(QJsonDocument(errorResp).toJson(QJsonDocument::Compact));
    socket->flush();
    return;
  }

  QJsonObject request = doc.object();
  QString machineId = request.value("machineId").toString().trimmed();
  QString licenseKey = request.value("licenseKey").toString().trimmed();

  qInfo() << "[Server] Querying DB for Key:" << licenseKey
          << "from MachineID:" << machineId;

  QSqlQuery query(m_db);
  query.prepare(
      "SELECT id, username, machine_id, days_remaining, plan_name, is_active "
      "FROM licenses WHERE license_key = :key AND is_active = 1");
  query.bindValue(":key", licenseKey);

  if (query.exec() && query.next()) {
    int id = query.value(0).toInt();
    QString username = query.value(1).toString();
    QString boundMid = query.value(2).toString();
    int days = query.value(3).toInt();
    QString plan = query.value(4).toString();

    // If key is not yet bound to a machine ID, bind it now to this machineId
    if (boundMid.isEmpty()) {
      QSqlQuery updateQuery(m_db);
      updateQuery.prepare(
          "UPDATE licenses SET machine_id = :mid WHERE id = :id");
      updateQuery.bindValue(":mid", machineId);
      updateQuery.bindValue(":id", id);
      updateQuery.exec();
      boundMid = machineId;
      qInfo() << "[Server] Bound Key" << licenseKey
              << "to MachineID:" << machineId;
    }

    // Verify machine ID match
    if (boundMid == machineId || boundMid == "*") {
      QJsonObject successResp;
      successResp["status"] = "valid";
      successResp["username"] = username;
      successResp["machineId"] = machineId;
      successResp["days"] = days;
      successResp["plan"] = plan;
      successResp["message"] =
          QString("Xác thực thành công! Xin chào %1").arg(username);

      qInfo() << "[Server] SUCCESS: Key valid for User:" << username << "("
              << days << "days)";
      socket->write(QJsonDocument(successResp).toJson(QJsonDocument::Compact));
      socket->flush();
      return;
    } else {
      QJsonObject failResp;
      failResp["status"] = "invalid";
      failResp["message"] =
          QString("Mã key này đã được kích hoạt trên thiết bị khác!");
      qWarning() << "[Server] FAILED: Key bound to another machine ("
                 << boundMid << ")";
      socket->write(QJsonDocument(failResp).toJson(QJsonDocument::Compact));
      socket->flush();
      return;
    }
  }

  // Key not found in database
  QJsonObject notFoundResp;
  notFoundResp["status"] = "invalid";
  notFoundResp["message"] =
      "Mã Product Key không tồn tại trên hệ thống database!";
  qWarning() << "[Server] FAILED: Key not found in DB:" << licenseKey;
  socket->write(QJsonDocument(notFoundResp).toJson(QJsonDocument::Compact));
  socket->flush();
}

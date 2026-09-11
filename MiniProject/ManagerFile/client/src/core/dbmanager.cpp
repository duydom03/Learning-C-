#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

DbManager::DbManager(QObject *parent)
    : QObject(parent)
{
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
        m_db.close();
}

bool DbManager::initDatabase(const QString &dbPath)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", "license_connection");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qCritical() << "[DB] Failed to open database:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery query(m_db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS license_attempts ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  timestamp TEXT NOT NULL,"
        "  license_key TEXT NOT NULL,"
        "  machine_id TEXT NOT NULL,"
        "  status TEXT NOT NULL,"
        "  days_remaining INTEGER DEFAULT 0"
        ")"
    );

    if (!ok) {
        qCritical() << "[DB] Failed to create table:" << query.lastError().text();
        return false;
    }

    qInfo() << "[DB] Database initialized:" << dbPath;
    return true;
}

void DbManager::logLicenseAttempt(const QString &licenseKey,
                                   const QString &machineId,
                                   const QString &status,
                                   int daysRemaining)
{
    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO license_attempts (timestamp, license_key, machine_id, status, days_remaining) "
        "VALUES (:ts, :key, :mid, :status, :days)"
    );
    query.bindValue(":ts",     QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(":key",    licenseKey);
    query.bindValue(":mid",    machineId);
    query.bindValue(":status", status);
    query.bindValue(":days",   daysRemaining);

    if (!query.exec()) {
        qWarning() << "[DB] Failed to log attempt:" << query.lastError().text();
    } else {
        qInfo() << "[DB] Logged attempt: status=" << status << "days=" << daysRemaining;
    }
}

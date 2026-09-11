#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = nullptr);
    ~DbManager();

    bool initDatabase(const QString &dbPath = "license.db");
    void logLicenseAttempt(const QString &licenseKey,
                           const QString &machineId,
                           const QString &status,
                           int daysRemaining);

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H

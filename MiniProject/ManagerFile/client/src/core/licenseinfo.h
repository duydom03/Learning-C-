#ifndef LICENSEINFO_H
#define LICENSEINFO_H

#include <QObject>
#include <QString>
#include <QDateTime>

class LicenseInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isValid READ isValid NOTIFY changed)
    Q_PROPERTY(QString customer READ customer NOTIFY changed)
    Q_PROPERTY(QString machineId READ machineId NOTIFY changed)
    Q_PROPERTY(QString plan READ plan NOTIFY changed)
    Q_PROPERTY(QString expirationDate READ expirationDate NOTIFY changed)
    Q_PROPERTY(int remainingDays READ remainingDays NOTIFY changed)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY changed)

public:
    explicit LicenseInfo(QObject *parent = nullptr);

    bool isValid() const;
    QString customer() const;
    QString machineId() const;
    QString plan() const;
    QString expirationDate() const;
    int remainingDays() const;
    QString statusMessage() const;

    void setValid(bool valid, const QString &customer, const QString &machineId,
                  const QString &plan, int days, const QString &message);
    void setError(const QString &message);
    void reset();

signals:
    void changed();

private:
    bool m_isValid = false;
    QString m_customer;
    QString m_machineId;
    QString m_plan;
    int m_remainingDays = 0;
    QDateTime m_expirationDate;
    QString m_statusMessage;
};

#endif // LICENSEINFO_H

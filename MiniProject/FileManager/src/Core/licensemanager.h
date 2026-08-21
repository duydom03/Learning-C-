#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVariantMap>

class LicenseManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool     isValid        READ isValid        NOTIFY licenseChanged)
    Q_PROPERTY(QString  customer       READ customer       NOTIFY licenseChanged)
    Q_PROPERTY(QString  email          READ email          NOTIFY licenseChanged)
    Q_PROPERTY(QString  productKey     READ productKey     NOTIFY licenseChanged)
    Q_PROPERTY(QString  edition        READ edition        NOTIFY licenseChanged)
    Q_PROPERTY(QDate    expirationDate READ expirationDate NOTIFY licenseChanged)
    Q_PROPERTY(int      remainingDays  READ remainingDays  NOTIFY licenseChanged)
    Q_PROPERTY(QString  activationDate READ activationDate NOTIFY licenseChanged)
    Q_PROPERTY(QString  statusMessage  READ statusMessage  NOTIFY licenseChanged)

public:
    explicit LicenseManager(QObject *parent = nullptr);

    bool     isValid()        const { return m_isValid; }
    QString  customer()       const { return m_customer; }
    QString  email()          const { return m_email; }
    QString  productKey()     const { return m_productKey; }
    QString  edition()        const { return m_edition; }
    QDate    expirationDate() const { return m_expirationDate; }
    int      remainingDays()  const;
    QString  activationDate() const { return m_activationDate; }
    QString  statusMessage()  const { return m_statusMessage; }

public slots:
    void activateLicense(const QString &key);
    void deactivateLicense();

signals:
    void licenseChanged();
    void activationSucceeded();
    void activationFailed(const QString &reason);

private:
    struct LicenseData {
        QString customer;
        QString email;
        QString edition;
        QDate   expirationDate;
    };

    QMap<QString, LicenseData> m_validKeys;

    bool    m_isValid        = false;
    QString m_customer;
    QString m_email;
    QString m_productKey;
    QString m_edition;
    QDate   m_expirationDate;
    QString m_activationDate;
    QString m_statusMessage;
};

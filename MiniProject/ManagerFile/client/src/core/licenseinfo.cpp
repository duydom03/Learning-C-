#include "licenseinfo.h"

LicenseInfo::LicenseInfo(QObject *parent)
    : QObject(parent)
{
}

bool LicenseInfo::isValid() const { return m_isValid; }
QString LicenseInfo::customer() const { return m_customer; }
QString LicenseInfo::machineId() const { return m_machineId; }
QString LicenseInfo::plan() const { return m_plan; }
int LicenseInfo::remainingDays() const { return m_remainingDays; }
QString LicenseInfo::statusMessage() const { return m_statusMessage; }

QString LicenseInfo::expirationDate() const
{
    if (!m_isValid) return QString();
    return m_expirationDate.toString("yyyy-MM-dd");
}

void LicenseInfo::setValid(bool valid, const QString &customer, const QString &machineId,
                            const QString &plan, int days, const QString &message)
{
    m_isValid = valid;
    m_customer = customer;
    m_machineId = machineId;
    m_plan = plan;
    m_remainingDays = days;
    m_expirationDate = QDateTime::currentDateTime().addDays(days);
    m_statusMessage = message;
    emit changed();
}

void LicenseInfo::setError(const QString &message)
{
    m_isValid = false;
    m_customer.clear();
    m_plan.clear();
    m_remainingDays = 0;
    m_statusMessage = message;
    emit changed();
}

void LicenseInfo::reset()
{
    m_isValid = false;
    m_customer.clear();
    m_machineId.clear();
    m_plan.clear();
    m_remainingDays = 0;
    m_statusMessage.clear();
    emit changed();
}

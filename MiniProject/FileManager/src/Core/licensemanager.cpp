#include "licensemanager.h"
#include <QDate>

LicenseManager::LicenseManager(QObject *parent)
    : QObject(parent)
{
    // Predefined valid license keys (in a real app, validate via server or crypto)
    m_validKeys["FM-2024-DEMO-KEY"] = {
        "FM Corp Demo",
        "demo@filemanager.io",
        "Professional",
        QDate(2025, 12, 31)
    };
    m_validKeys["FM-PRO-ENTERPRISE-2025"] = {
        "Công ty TNHH ABC Technology",
        "admin@abc-tech.vn",
        "Enterprise",
        QDate(2026, 6, 30)
    };
    m_validKeys["FM-TEAM-VN-2025X"] = {
        "Nguyễn Văn An",
        "an.nguyen@company.com",
        "Team (10 seats)",
        QDate(2025, 9, 15)
    };
}

int LicenseManager::remainingDays() const
{
    if (!m_isValid) return 0;
    return QDate::currentDate().daysTo(m_expirationDate);
}

void LicenseManager::activateLicense(const QString &key)
{
    const QString trimmedKey = key.trimmed().toUpper();

    if (trimmedKey.isEmpty()) {
        m_statusMessage = "Vui lòng nhập Product Key.";
        emit licenseChanged();
        emit activationFailed(m_statusMessage);
        return;
    }

    if (!m_validKeys.contains(trimmedKey)) {
        m_isValid       = false;
        m_statusMessage = "Product Key không hợp lệ hoặc đã hết hạn.";
        emit licenseChanged();
        emit activationFailed(m_statusMessage);
        return;
    }

    const LicenseData &data = m_validKeys[trimmedKey];

    if (QDate::currentDate() > data.expirationDate) {
        m_isValid       = false;
        m_statusMessage = "License này đã hết hạn vào " + data.expirationDate.toString("dd/MM/yyyy") + ".";
        emit licenseChanged();
        emit activationFailed(m_statusMessage);
        return;
    }

    m_isValid         = true;
    m_productKey      = trimmedKey;
    m_customer        = data.customer;
    m_email           = data.email;
    m_edition         = data.edition;
    m_expirationDate  = data.expirationDate;
    m_activationDate  = QDate::currentDate().toString("dd/MM/yyyy");
    m_statusMessage   = "Kích hoạt thành công!";

    emit licenseChanged();
    emit activationSucceeded();
}

void LicenseManager::deactivateLicense()
{
    m_isValid        = false;
    m_customer       = "";
    m_email          = "";
    m_productKey     = "";
    m_edition        = "";
    m_expirationDate = QDate();
    m_activationDate = "";
    m_statusMessage  = "";
    emit licenseChanged();
}

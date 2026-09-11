#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QString>
#include <QFutureWatcher>
#include "licenseinfo.h"
#include "dbmanager.h"
#include "licenseclient.h"
#include "filemodel.h"
#include "foldermodel.h"
#include "threadmodel.h"

class AppCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LicenseInfo* licenseInfo READ licenseInfo NOTIFY licenseInfoChanged)
    Q_PROPERTY(FileModel* fileModel READ fileModel CONSTANT)
    Q_PROPERTY(FolderModel* folderModel READ folderModel CONSTANT)
    Q_PROPERTY(ThreadModel* threadModel READ threadModel CONSTANT)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QString busyMessage READ busyMessage NOTIFY busyChanged)
    Q_PROPERTY(int busyProgress READ busyProgress NOTIFY busyChanged)
    Q_PROPERTY(QString viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)

public:
    explicit AppCore(QObject *parent = nullptr);
    ~AppCore();

    LicenseInfo* licenseInfo() const;
    FileModel* fileModel() const;
    FolderModel* folderModel() const;
    ThreadModel* threadModel() const;

    bool isBusy() const;
    QString busyMessage() const;
    int busyProgress() const;
    QString viewMode() const;
    void setViewMode(const QString &mode);

public slots:
    void activateLicense(const QString &key);
    void upload();
    void startUploadTask(const QString &filePath);
    void createFolder(const QString &name);
    void refresh();
    void search(const QString &keyword);
    void goBack();
    void goForward();
    void copyToClipboard(const QString &text);
    void toggleView();
    void menuAction(const QString &action, const QString &itemPath);
    void cancelUpload(const QString &taskId);
    QString readLogFile();
    void logOperation(const QString &message);
    void setBusy(bool busy, const QString &message = QString(), int progress = 0);
    void logout();

signals:
    void licenseInfoChanged();
    void busyChanged();
    void viewModeChanged();
    void showToast(const QString &message, bool isError);

private:
    LicenseInfo *m_licenseInfo;
    DbManager *m_dbManager;
    LicenseClient *m_licenseClient;
    FileModel *m_fileModel;
    FolderModel *m_folderModel;
    ThreadModel *m_threadModel;

    bool m_isBusy = false;
    QString m_busyMessage;
    int m_busyProgress = 0;
    QString m_viewMode = "grid"; // "grid" or "list"

    QString m_cloudRoot;
    int m_taskCounter = 0;


};

#endif // APPCORE_H

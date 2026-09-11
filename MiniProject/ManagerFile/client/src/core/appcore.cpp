#include "appcore.h"
#include "uploadtask.h"

#include <QGuiApplication>
#include <QClipboard>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

AppCore::AppCore(QObject *parent)
    : QObject(parent)
    , m_licenseInfo(new LicenseInfo(this))
    , m_dbManager(new DbManager(this))
    , m_licenseClient(new LicenseClient(m_licenseInfo, m_dbManager, this))
    , m_fileModel(new FileModel(this))
    , m_folderModel(new FolderModel(this))
    , m_threadModel(new ThreadModel(this))
{
    // Initialize SQLite database for license logs
    m_dbManager->initDatabase("license.db");

    // Set cloud_root path inside application executable directory
    m_cloudRoot = QCoreApplication::applicationDirPath() + "/cloud_root";
    m_fileModel->setRootPath(m_cloudRoot);
    m_folderModel->setCloudRoot(m_cloudRoot);

    // Forward fileModel operation results as toast signals
    connect(m_fileModel, &FileModel::operationResult, this, [this](const QString &msg, bool success) {
        logOperation(msg);
        emit showToast(msg, !success);
    });

    logOperation("AppCore initialized. Machine ID: " + m_licenseClient->machineId());
}

AppCore::~AppCore()
{
}

LicenseInfo* AppCore::licenseInfo() const { return m_licenseInfo; }
FileModel* AppCore::fileModel() const { return m_fileModel; }
FolderModel* AppCore::folderModel() const { return m_folderModel; }
ThreadModel* AppCore::threadModel() const { return m_threadModel; }

bool AppCore::isBusy() const { return m_isBusy; }
QString AppCore::busyMessage() const { return m_busyMessage; }
int AppCore::busyProgress() const { return m_busyProgress; }
QString AppCore::viewMode() const { return m_viewMode; }

void AppCore::setViewMode(const QString &mode)
{
    if (m_viewMode != mode) {
        m_viewMode = mode;
        emit viewModeChanged();
    }
}

void AppCore::activateLicense(const QString &key)
{
    logOperation("Activating license with key: " + key);
    m_licenseClient->verifyKey(key);
}

void AppCore::upload()
{
    // Open QFileDialog to select a file from local system
    QString selectedFile = QFileDialog::getOpenFileName(
        nullptr,
        "Chọn file để tải lên Cloud Drive",
        QDir::homePath(),
        "All Files (*.*)");

    if (!selectedFile.isEmpty()) {
        startUploadTask(selectedFile);
    }
}

void AppCore::startUploadTask(const QString &filePath)
{
    QFileInfo fi(filePath);
    m_taskCounter++;
    QString taskId = QString("TASK-%1").arg(m_taskCounter, 3, 10, QChar('0'));
    qint64 sizeKb = fi.exists() ? (fi.size() / 1024) : 8500; // default simulate ~8.5MB if fake

    logOperation(QString("Started upload task [%1]: %2 (%3 KB)")
                     .arg(taskId, fi.fileName()).arg(sizeKb));

    // UploadTask runs in background QThread (simulating 2-minute delay for large files)
    UploadTask *task = new UploadTask(taskId, fi.fileName(), sizeKb, m_fileModel->currentPath(), this);
    m_threadModel->addUploadTask(task);

    // Refresh file model once completed
    connect(task, &UploadTask::taskFinished, this, [this, taskId](const QString &id, bool success) {
        Q_UNUSED(id)
        if (success) {
            m_fileModel->refresh();
            logOperation("Upload task [" + taskId + "] completed successfully.");
            emit showToast("Tải lên hoàn tất!", false);
        } else {
            logOperation("Upload task [" + taskId + "] was cancelled.");
            emit showToast("Đã hủy tải lên", true);
        }
    });
}

void AppCore::createFolder(const QString &name)
{
    logOperation("Create folder requested: " + name);
    m_fileModel->createFolder(name);
}

void AppCore::refresh()
{
    logOperation("Refreshing directory view");
    m_fileModel->refresh();
}

void AppCore::search(const QString &keyword)
{
    m_fileModel->searchFiles(keyword, "All");
}

void AppCore::goBack()
{
    logOperation("Navigation goBack requested");
    m_fileModel->goBack();
}

void AppCore::goForward()
{
    logOperation("Navigation goForward requested");
    m_fileModel->goForward();
}

void AppCore::copyToClipboard(const QString &text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard) {
        clipboard->setText(text);
        emit showToast("Đã sao chép vào bộ nhớ tạm!", false);
    }
}

void AppCore::toggleView()
{
    setViewMode(m_viewMode == "grid" ? "list" : "grid");
}

void AppCore::menuAction(const QString &action, const QString &itemPath)
{
    QFileInfo fi(itemPath.isEmpty() ? m_fileModel->currentPath() : itemPath);
    QString name = fi.fileName();

    logOperation(QString("Menu action [%1] on: %2").arg(action, name));

    if (action == "Delete") {
        m_fileModel->deleteItem(name);
    } else if (action == "Rename") {
        // Handled via QML prompt or default rename
        emit showToast("Nhấn F2 để đổi tên tệp/thư mục", false);
    } else if (action == "Properties") {
        QString details = QString("Tên: %1\nLoại: %2\nPath: %3")
                              .arg(name)
                              .arg(fi.isDir() ? "Thư mục" : "Tệp tin")
                              .arg(fi.absoluteFilePath());
        emit showToast(details, false);
    } else if (action == "Refresh") {
        refresh();
    }
}

void AppCore::cancelUpload(const QString &taskId)
{
    logOperation("Cancel requested for upload task: " + taskId);
    m_threadModel->cancelTask(taskId);
}

QString AppCore::readLogFile()
{
    QString logPath = QCoreApplication::applicationDirPath() + "/filemanager.log";
    QFile file(logPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "Chưa có dữ liệu nhật ký thao tác (filemanager.log).";
    }
    QTextStream in(&file);
    return in.readAll();
}

void AppCore::logOperation(const QString &message)
{
    QString logPath = QCoreApplication::applicationDirPath() + "/filemanager.log";
    QFile file(logPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "] "
            << message << "\n";
        file.close();
    }
    qInfo() << "[Log]" << message;
}

void AppCore::setBusy(bool busy, const QString &message, int progress)
{
    m_isBusy = busy;
    m_busyMessage = message;
    m_busyProgress = progress;
    emit busyChanged();
}

void AppCore::logout()
{
    logOperation("User requested logout.");
    m_licenseInfo->reset();
    emit showToast("Đã đăng xuất khỏi tài khoản!", false);
}

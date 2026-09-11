#include "filemodel.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDirIterator>
#include <QDebug>

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.count();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.count())
        return QVariant();

    const FileItem &item = m_items.at(index.row());

    switch (role) {
    case FileNameRole:     return item.name;
    case FileSizeRole:     return item.size;
    case FileModifiedRole: return item.modified;
    case FileIconRole:     return item.iconId;
    case IsFolderRole:     return item.isFolder;
    case FilePathRole:     return item.fullPath;
    default:               return QVariant();
    }
}

QHash<int, QByteArray> FileModel::roleNames() const
{
    return {
        {FileNameRole,     "fileName"},
        {FileSizeRole,     "fileSize"},
        {FileModifiedRole, "fileModified"},
        {FileIconRole,     "fileIcon"},
        {IsFolderRole,     "isFolder"},
        {FilePathRole,     "filePath"}
    };
}

QString FileModel::currentPath() const
{
    return m_currentPath;
}

bool FileModel::isRoot() const
{
    return m_currentPath == m_rootPath;
}

void FileModel::setRootPath(const QString &rootPath)
{
    m_rootPath = rootPath;

    // Ensure cloud_root directory exists
    QDir dir(rootPath);
    if (!dir.exists()) {
        dir.mkpath(".");
        qInfo() << "[FileModel] Created cloud root:" << rootPath;
    }

    browseTo(rootPath);
}

bool FileModel::canGoBack() const
{
    return m_historyIndex > 0;
}

bool FileModel::canGoForward() const
{
    return m_historyIndex >= 0 && m_historyIndex < m_history.count() - 1;
}

void FileModel::browseTo(const QString &path)
{
    QString resolvedPath = path;
    QDir dir(resolvedPath);
    if (!dir.exists()) {
        emit operationResult("Thư mục không tồn tại: " + resolvedPath, false);
        return;
    }

    QString newPath = dir.absolutePath();

    // If navigating to a new path (not history back/forward), trim forward history and append
    if (m_currentPath != newPath) {
        if (m_historyIndex >= 0 && m_historyIndex < m_history.count() - 1) {
            m_history = m_history.mid(0, m_historyIndex + 1);
        }
        m_history.append(newPath);
        m_historyIndex = m_history.count() - 1;
        emit historyChanged();
    }

    m_currentPath = newPath;
    loadDirectory(m_currentPath);
    emit currentPathChanged();
}

void FileModel::goBack()
{
    if (!canGoBack()) return;

    m_historyIndex--;
    m_currentPath = m_history.at(m_historyIndex);
    loadDirectory(m_currentPath);
    emit currentPathChanged();
    emit historyChanged();
}

void FileModel::goForward()
{
    if (!canGoForward()) return;

    m_historyIndex++;
    m_currentPath = m_history.at(m_historyIndex);
    loadDirectory(m_currentPath);
    emit currentPathChanged();
    emit historyChanged();
}

void FileModel::refresh()
{
    loadDirectory(m_currentPath);
}

void FileModel::createFolder(const QString &folderName)
{
    if (folderName.trimmed().isEmpty()) {
        emit operationResult("Tên thư mục không được để trống", false);
        return;
    }

    QDir dir(m_currentPath);
    if (dir.mkdir(folderName)) {
        qInfo() << "[FileModel] Created folder:" << folderName;
        refresh();
        emit operationResult("Tạo thư mục thành công: " + folderName, true);
    } else {
        emit operationResult("Không thể tạo thư mục: " + folderName, false);
    }
}

void FileModel::createFile(const QString &fileName, const QString &content)
{
    if (fileName.trimmed().isEmpty()) {
        emit operationResult("Tên file không được để trống", false);
        return;
    }

    QString filePath = m_currentPath + "/" + fileName;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << content;
        file.close();
        qInfo() << "[FileModel] Created file:" << fileName;
        refresh();
        emit operationResult("Tạo file thành công: " + fileName, true);
    } else {
        emit operationResult("Không thể tạo file: " + fileName, false);
    }
}

void FileModel::renameItem(const QString &oldName, const QString &newName)
{
    if (newName.trimmed().isEmpty()) {
        emit operationResult("Tên mới không được để trống", false);
        return;
    }

    QDir dir(m_currentPath);
    if (dir.rename(oldName, newName)) {
        qInfo() << "[FileModel] Renamed:" << oldName << "->" << newName;
        refresh();
        emit operationResult("Đổi tên thành công", true);
    } else {
        emit operationResult("Không thể đổi tên: " + oldName, false);
    }
}

void FileModel::deleteItem(const QString &name)
{
    QString fullPath = m_currentPath + "/" + name;
    QFileInfo fi(fullPath);

    bool ok = false;
    if (fi.isDir()) {
        QDir dir(fullPath);
        ok = dir.removeRecursively();
    } else {
        ok = QFile::remove(fullPath);
    }

    if (ok) {
        qInfo() << "[FileModel] Deleted:" << name;
        refresh();
        emit operationResult("Xóa thành công: " + name, true);
    } else {
        emit operationResult("Không thể xóa: " + name, false);
    }
}

void FileModel::searchFiles(const QString &query, const QString &filterType)
{
    if (query.trimmed().isEmpty()) {
        loadDirectory(m_currentPath);
        return;
    }

    beginResetModel();
    m_items.clear();

    QDirIterator it(m_currentPath,
                    QDir::AllEntries | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        QFileInfo fi = it.fileInfo();

        if (!fi.fileName().contains(query, Qt::CaseInsensitive))
            continue;

        if (!matchesFilter(fi, filterType))
            continue;

        FileItem item;
        item.name     = fi.fileName();
        item.isFolder = fi.isDir();
        item.size     = fi.isDir() ? "--" : formatFileSize(fi.size());
        item.modified = fi.lastModified().toString("yyyy-MM-dd hh:mm");
        item.iconId   = iconIdForFile(fi);
        item.fullPath = fi.absoluteFilePath();
        m_items.append(item);
    }

    endResetModel();
    emit countChanged();
    qInfo() << "[FileModel] Search:" << query << "found" << m_items.count() << "items";
}

void FileModel::loadDirectory(const QString &path)
{
    beginResetModel();
    m_items.clear();

    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(
        QDir::AllEntries | QDir::NoDotAndDotDot,
        QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);

    for (const QFileInfo &fi : entries) {
        FileItem item;
        item.name     = fi.fileName();
        item.isFolder = fi.isDir();
        item.size     = fi.isDir() ? "--" : formatFileSize(fi.size());
        item.modified = fi.lastModified().toString("yyyy-MM-dd hh:mm");
        item.iconId   = iconIdForFile(fi);
        item.fullPath = fi.absoluteFilePath();
        m_items.append(item);
    }

    endResetModel();
    emit countChanged();
}

QString FileModel::formatFileSize(qint64 bytes) const
{
    if (bytes < 1024)             return QString::number(bytes) + " B";
    if (bytes < 1024 * 1024)      return QString::number(bytes / 1024.0, 'f', 1) + " KB";
    if (bytes < 1024 * 1024 * 1024) return QString::number(bytes / (1024.0 * 1024.0), 'f', 1) + " MB";
    return QString::number(bytes / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}

QString FileModel::iconIdForFile(const QFileInfo &fi) const
{
    if (fi.isDir()) return "folder";

    QString suffix = fi.suffix().toLower();
    if (suffix == "txt" || suffix == "md" || suffix == "log")
        return "document-text";
    if (suffix == "pdf")
        return "document-pdf";
    if (suffix == "doc" || suffix == "docx")
        return "document-word";
    if (suffix == "xls" || suffix == "xlsx")
        return "document-excel";
    if (suffix == "png" || suffix == "jpg" || suffix == "jpeg" || suffix == "bmp" || suffix == "gif" || suffix == "svg")
        return "image";
    if (suffix == "mp3" || suffix == "wav" || suffix == "flac")
        return "audio";
    if (suffix == "mp4" || suffix == "avi" || suffix == "mkv")
        return "video";
    if (suffix == "zip" || suffix == "rar" || suffix == "7z" || suffix == "tar" || suffix == "gz")
        return "archive";

    return "file-generic";
}

bool FileModel::matchesFilter(const QFileInfo &fi, const QString &filterType) const
{
    if (filterType == "All") return true;
    if (filterType == "Folders") return fi.isDir();

    QString suffix = fi.suffix().toLower();
    if (filterType == "Documents")
        return (suffix == "txt" || suffix == "pdf" || suffix == "doc" || suffix == "docx" || suffix == "md" || suffix == "log");
    if (filterType == "Images")
        return (suffix == "png" || suffix == "jpg" || suffix == "jpeg" || suffix == "bmp" || suffix == "gif" || suffix == "svg");

    return true;
}

#include "foldermodel.h"
#include <QDir>
#include <QDebug>

FolderModel::FolderModel(QObject *parent)
    : QFileSystemModel(parent)
{
    // Filter to only display directories (exclude files, dot, dot-dot)
    setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
}

void FolderModel::setCloudRoot(const QString &rootPath)
{
    QDir dir(rootPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    setRootPath(dir.absolutePath());
    qInfo() << "[FolderModel] Root path set to:" << dir.absolutePath();
}

QHash<int, QByteArray> FolderModel::roleNames() const
{
    QHash<int, QByteArray> roles = QFileSystemModel::roleNames();
    roles[QFileSystemModel::FileNameRole] = "fileName";
    roles[QFileSystemModel::FilePathRole] = "filePath";
    return roles;
}

#include "filemodel.h"

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent)
{
    populateDemoData("/MyDrive");
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_entries.count();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_entries.count())
        return {};

    const FileEntry &e = m_entries.at(index.row());
    switch (role) {
    case FileNameRole:     return e.fileName;
    case FileSizeRole:     return e.fileSize;
    case FileModifiedRole: return e.fileModified;
    case FileIconRole:     return e.fileIcon;
    case IsFolderRole:     return e.isFolder;
    case FilePathRole:     return e.filePath;
    case FileIdRole:       return e.id;
    }
    return {};
}

QHash<int, QByteArray> FileModel::roleNames() const
{
    return {
        { FileNameRole,     "fileName"     },
        { FileSizeRole,     "fileSize"     },
        { FileModifiedRole, "fileModified" },
        { FileIconRole,     "fileIcon"     },
        { IsFolderRole,     "isFolder"     },
        { FilePathRole,     "filePath"     },
        { FileIdRole,       "fileId"       },
    };
}

void FileModel::loadFolder(const QString &path)
{
    m_currentPath = path;
    populateDemoData(path);
}

void FileModel::searchFiles(const QString &keyword)
{
    if (keyword.isEmpty()) {
        populateDemoData(m_currentPath);
        return;
    }
    beginResetModel();
    QList<FileEntry> all;
    populateDemoData(m_currentPath);
    for (const auto &e : m_entries) {
        if (e.fileName.contains(keyword, Qt::CaseInsensitive))
            all.append(e);
    }
    m_entries = all;
    endResetModel();
}

void FileModel::deleteFile(const QString &path)
{
    for (int i = 0; i < m_entries.count(); ++i) {
        if (m_entries[i].filePath == path) {
            beginRemoveRows({}, i, i);
            m_entries.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

void FileModel::renameFile(const QString &path, const QString &newName)
{
    for (int i = 0; i < m_entries.count(); ++i) {
        if (m_entries[i].filePath == path) {
            m_entries[i].fileName = newName;
            const QModelIndex idx = index(i);
            emit dataChanged(idx, idx, { FileNameRole });
            return;
        }
    }
}

void FileModel::createFolder(const QString &parentPath, const QString &name)
{
    FileEntry e;
    e.id           = QStringLiteral("folder-%1").arg(m_entries.count());
    e.fileName     = name;
    e.fileSize     = QStringLiteral("—");
    e.fileModified = QStringLiteral("Vừa xong");
    e.fileIcon     = QStringLiteral("folder");
    e.isFolder     = true;
    e.filePath     = parentPath + "/" + name;

    beginInsertRows({}, 0, 0);
    m_entries.prepend(e);
    endInsertRows();
}

void FileModel::refreshCurrentFolder()
{
    populateDemoData(m_currentPath);
}

void FileModel::populateDemoData(const QString &path)
{
    beginResetModel();
    m_entries.clear();

    QList<FileEntry> demo = {
        { "f1",  "Tài liệu dự án 2024",       "—",        "20/08/2024", "folder",      true,  path + "/Tài liệu dự án 2024" },
        { "f2",  "Ảnh sản phẩm",               "—",        "18/08/2024", "folder",      true,  path + "/Ảnh sản phẩm"        },
        { "f3",  "Backup Q3",                  "—",        "15/08/2024", "folder",      true,  path + "/Backup Q3"            },
        { "f4",  "Báo cáo tài chính Q3.xlsx",  "2.4 MB",   "19/08/2024", "spreadsheet", false, path + "/Báo cáo tài chính Q3.xlsx" },
        { "f5",  "Hợp đồng dịch vụ 2024.pdf",  "845 KB",   "17/08/2024", "pdf",         false, path + "/Hợp đồng dịch vụ 2024.pdf" },
        { "f6",  "Thiết kế UI v3.2.png",        "5.1 MB",   "20/08/2024", "image",       false, path + "/Thiết kế UI v3.2.png"      },
        { "f7",  "Hướng dẫn sử dụng.docx",      "1.2 MB",   "14/08/2024", "document",    false, path + "/Hướng dẫn sử dụng.docx"    },
        { "f8",  "database_backup_aug.zip",     "128 MB",   "16/08/2024", "archive",     false, path + "/database_backup_aug.zip"    },
        { "f9",  "main.py",                     "24 KB",    "20/08/2024", "code",        false, path + "/main.py"                    },
        { "f10", "Demo presentation.mp4",       "312 MB",   "12/08/2024", "video",       false, path + "/Demo presentation.mp4"      },
        { "f11", "Danh sách khách hàng.xlsx",   "1.8 MB",   "11/08/2024", "spreadsheet", false, path + "/Danh sách khách hàng.xlsx"  },
        { "f12", "logo_final.png",              "256 KB",   "09/08/2024", "image",       false, path + "/logo_final.png"             },
    };

    m_entries = demo;
    endResetModel();
}

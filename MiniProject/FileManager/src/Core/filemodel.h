#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QDateTime>

struct FileEntry {
    QString  id;
    QString  fileName;
    QString  fileSize;
    QString  fileModified;
    QString  fileIcon;   // "folder","image","document","spreadsheet","video","archive","pdf","code"
    bool     isFolder;
    QString  filePath;
};

class FileModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum FileRoles {
        FileNameRole     = Qt::UserRole + 1,
        FileSizeRole,
        FileModifiedRole,
        FileIconRole,
        IsFolderRole,
        FilePathRole,
        FileIdRole
    };
    Q_ENUM(FileRoles)

    explicit FileModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void loadFolder(const QString &path);
    void searchFiles(const QString &keyword);
    void deleteFile(const QString &path);
    void renameFile(const QString &path, const QString &newName);
    void createFolder(const QString &parentPath, const QString &name);
    void refreshCurrentFolder();

private:
    void populateDemoData(const QString &path);

    QList<FileEntry> m_entries;
    QString          m_currentPath;
};

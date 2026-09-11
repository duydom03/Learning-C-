#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QDir>
#include <QList>

struct FileItem {
    QString name;
    QString size;
    QString modified;
    QString iconId;
    bool isFolder;
    QString fullPath;
};

class FileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
    Q_PROPERTY(bool isRoot READ isRoot NOTIFY currentPathChanged)
    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY historyChanged)
    Q_PROPERTY(bool canGoForward READ canGoForward NOTIFY historyChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        FileNameRole = Qt::UserRole + 1,
        FileSizeRole,
        FileModifiedRole,
        FileIconRole,
        IsFolderRole,
        FilePathRole
    };

    explicit FileModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString currentPath() const;
    bool isRoot() const;
    bool canGoBack() const;
    bool canGoForward() const;

    void setRootPath(const QString &rootPath);

public slots:
    void browseTo(const QString &path);
    void goBack();
    void goForward();
    void refresh();
    void createFolder(const QString &folderName);
    void createFile(const QString &fileName, const QString &content = QString());
    void renameItem(const QString &oldName, const QString &newName);
    void deleteItem(const QString &name);
    void searchFiles(const QString &query, const QString &filterType = "All");

signals:
    void currentPathChanged();
    void historyChanged();
    void countChanged();
    void operationResult(const QString &message, bool success);

private:
    void loadDirectory(const QString &path);
    QString formatFileSize(qint64 bytes) const;
    QString iconIdForFile(const QFileInfo &fi) const;
    bool matchesFilter(const QFileInfo &fi, const QString &filterType) const;

    QString m_rootPath;
    QString m_currentPath;
    QList<FileItem> m_items;

    QList<QString> m_history;
    int m_historyIndex = -1;
};

#endif // FILEMODEL_H

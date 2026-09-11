#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QFileSystemModel>

class FolderModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FolderModel(QObject *parent = nullptr);

    void setCloudRoot(const QString &rootPath);

    // Override roleNames so QML TreeView can access columns
    QHash<int, QByteArray> roleNames() const override;
};

#endif // FOLDERMODEL_H

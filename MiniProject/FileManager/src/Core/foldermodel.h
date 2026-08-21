#pragma once

#include <QAbstractItemModel>
#include <QList>
#include <QString>

struct FolderNode {
    QString         id;
    QString         name;
    QString         path;
    FolderNode     *parent  = nullptr;
    QList<FolderNode *> children;
    bool            expanded = false;

    ~FolderNode() { qDeleteAll(children); }
};

class FolderModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum FolderRoles {
        NameRole     = Qt::UserRole + 1,
        PathRole,
        ExpandedRole,
        HasChildrenRole,
        DepthRole
    };
    Q_ENUM(FolderRoles)

    explicit FolderModel(QObject *parent = nullptr);
    ~FolderModel() override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child)    const override;
    int rowCount   (const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data  (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void toggleExpanded(const QModelIndex &index);
    int  depthOf(const QModelIndex &index) const;

private:
    void buildDemoTree();

    FolderNode *m_root = nullptr;

    FolderNode *nodeFromIndex(const QModelIndex &index) const;
};

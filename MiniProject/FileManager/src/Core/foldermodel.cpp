#include "foldermodel.h"

FolderModel::FolderModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    buildDemoTree();
}

FolderModel::~FolderModel()
{
    delete m_root;
}

void FolderModel::buildDemoTree()
{
    m_root = new FolderNode{ "root_hidden", "", "", nullptr, {}, true };

    auto *myDrive = new FolderNode{ "mydrive", "My Drive", "/MyDrive", m_root, {}, true };
    m_root->children.append(myDrive);

    auto *docs = new FolderNode{ "docs", "Tài liệu dự án 2024", "/MyDrive/Tài liệu dự án 2024", myDrive };
    docs->children.append(new FolderNode{ "q1", "Q1 2024", "/MyDrive/Tài liệu dự án 2024/Q1 2024", docs });
    docs->children.append(new FolderNode{ "q2", "Q2 2024", "/MyDrive/Tài liệu dự án 2024/Q2 2024", docs });
    docs->children.append(new FolderNode{ "q3", "Q3 2024", "/MyDrive/Tài liệu dự án 2024/Q3 2024", docs });
    myDrive->children.append(docs);

    auto *images = new FolderNode{ "images", "Ảnh sản phẩm", "/MyDrive/Ảnh sản phẩm", myDrive };
    images->children.append(new FolderNode{ "web", "Website",    "/MyDrive/Ảnh sản phẩm/Website",    images });
    images->children.append(new FolderNode{ "app", "Mobile App", "/MyDrive/Ảnh sản phẩm/Mobile App", images });
    myDrive->children.append(images);

    myDrive->children.append(new FolderNode{ "backup", "Backup Q3", "/MyDrive/Backup Q3", myDrive });

    auto *shared = new FolderNode{ "shared", "Shared with me", "/Shared", m_root };
    shared->children.append(new FolderNode{ "team", "Team Resources", "/Shared/Team Resources", shared });
    m_root->children.append(shared);

    m_root->children.append(new FolderNode{ "trash", "Thùng rác", "/Trash", m_root });
}

FolderNode *FolderModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_root;
    return static_cast<FolderNode *>(index.internalPointer());
}

QModelIndex FolderModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) return {};
    FolderNode *parentNode = nodeFromIndex(parent);
    if (row < parentNode->children.count())
        return createIndex(row, column, parentNode->children.at(row));
    return {};
}

QModelIndex FolderModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) return {};
    FolderNode *node       = nodeFromIndex(child);
    FolderNode *parentNode = node->parent;
    if (!parentNode || parentNode == m_root) return {};
    FolderNode *grandParent = parentNode->parent;
    if (!grandParent) return {};
    int row = grandParent->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int FolderModel::rowCount(const QModelIndex &parent) const
{
    FolderNode *node = nodeFromIndex(parent);
    if (!node->expanded && parent.isValid()) return 0;
    return node->children.count();
}

int FolderModel::columnCount(const QModelIndex &) const { return 1; }

QVariant FolderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    FolderNode *node = nodeFromIndex(index);
    switch (role) {
    case Qt::DisplayRole:
    case NameRole:        return node->name;
    case PathRole:        return node->path;
    case ExpandedRole:    return node->expanded;
    case HasChildrenRole: return !node->children.isEmpty();
    case DepthRole:       return depthOf(index);
    }
    return {};
}

QHash<int, QByteArray> FolderModel::roleNames() const
{
    return {
        { NameRole,        "folderName"     },
        { PathRole,        "folderPath"     },
        { ExpandedRole,    "folderExpanded" },
        { HasChildrenRole, "hasChildren"    },
        { DepthRole,       "depth"          },
    };
}

void FolderModel::toggleExpanded(const QModelIndex &index)
{
    if (!index.isValid()) return;
    FolderNode *node = nodeFromIndex(index);
    if (node->children.isEmpty()) return;

    if (node->expanded) {
        beginRemoveRows(index, 0, node->children.count() - 1);
        node->expanded = false;
        endRemoveRows();
    } else {
        beginInsertRows(index, 0, node->children.count() - 1);
        node->expanded = true;
        endInsertRows();
    }
    emit dataChanged(index, index, { ExpandedRole });
}

int FolderModel::depthOf(const QModelIndex &index) const
{
    int depth = 0;
    QModelIndex p = parent(index);
    while (p.isValid()) { ++depth; p = parent(p); }
    return depth;
}

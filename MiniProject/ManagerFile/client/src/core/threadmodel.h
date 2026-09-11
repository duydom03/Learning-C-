#ifndef THREADMODEL_H
#define THREADMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QMap>

class UploadTask;

struct ThreadInfo {
    QString taskId;
    QString threadName;
    QString priority;
    double cpuUsage;
    double ramUsage;
    int progress;
    QString status;
    UploadTask *taskPtr = nullptr;
};

class ThreadModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int activeCount READ activeCount NOTIFY activeCountChanged)

public:
    enum Roles {
        TaskIdRole = Qt::UserRole + 1,
        ThreadNameRole,
        PriorityRole,
        CpuUsageRole,
        RamUsageRole,
        ProgressRole,
        StatusRole
    };

    explicit ThreadModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int activeCount() const;

    void addUploadTask(UploadTask *task);

public slots:
    void cancelTask(const QString &taskId);
    void updateTaskProgress(const QString &taskId, int progress, const QString &speed);
    void onTaskFinished(const QString &taskId, bool success);

signals:
    void activeCountChanged();
    void allTasksFinished();

private:
    QList<ThreadInfo> m_threads;
};

#endif // THREADMODEL_H

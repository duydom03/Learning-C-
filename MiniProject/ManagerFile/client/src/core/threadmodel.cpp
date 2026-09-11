#include "threadmodel.h"
#include "uploadtask.h"
#include <QRandomGenerator>
#include <QDebug>

ThreadModel::ThreadModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ThreadModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_threads.count();
}

QVariant ThreadModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_threads.count())
        return QVariant();

    const ThreadInfo &info = m_threads.at(index.row());

    switch (role) {
    case TaskIdRole:     return info.taskId;
    case ThreadNameRole: return info.threadName;
    case PriorityRole:   return info.priority;
    case CpuUsageRole:   return info.cpuUsage;
    case RamUsageRole:   return info.ramUsage;
    case ProgressRole:   return info.progress;
    case StatusRole:     return info.status;
    default:             return QVariant();
    }
}

QHash<int, QByteArray> ThreadModel::roleNames() const
{
    return {
        {TaskIdRole,     "taskId"},
        {ThreadNameRole, "threadName"},
        {PriorityRole,   "priority"},
        {CpuUsageRole,   "cpuUsage"},
        {RamUsageRole,   "ramUsage"},
        {ProgressRole,   "progress"},
        {StatusRole,     "status"}
    };
}

int ThreadModel::activeCount() const
{
    int count = 0;
    for (const ThreadInfo &info : m_threads) {
        if (info.status == "Running") count++;
    }
    return count;
}

void ThreadModel::addUploadTask(UploadTask *task)
{
    beginInsertRows(QModelIndex(), m_threads.count(), m_threads.count());

    ThreadInfo info;
    info.taskId     = task->taskId();
    info.threadName = "UploadWorker-" + task->fileName();
    info.priority   = "Normal";
    info.cpuUsage   = 2.0 + (QRandomGenerator::global()->generateDouble() * 6.5);
    info.ramUsage   = 15.0 + (QRandomGenerator::global()->generateDouble() * 30.0);
    info.progress   = 0;
    info.status     = "Running";
    info.taskPtr    = task;

    m_threads.append(info);
    endInsertRows();

    connect(task, &UploadTask::progressUpdated, this, &ThreadModel::updateTaskProgress);
    connect(task, &UploadTask::taskFinished, this, &ThreadModel::onTaskFinished);

    emit activeCountChanged();
    task->start();
}

void ThreadModel::cancelTask(const QString &taskId)
{
    for (int i = 0; i < m_threads.count(); ++i) {
        if (m_threads[i].taskId == taskId) {
            if (m_threads[i].taskPtr && m_threads[i].taskPtr->isRunning()) {
                m_threads[i].taskPtr->cancel();
                m_threads[i].status = "Cancelling...";
                QModelIndex idx = createIndex(i, 0);
                emit dataChanged(idx, idx, {StatusRole});
            }
            break;
        }
    }
}

void ThreadModel::updateTaskProgress(const QString &taskId, int progress, const QString &speed)
{
    Q_UNUSED(speed)
    for (int i = 0; i < m_threads.count(); ++i) {
        if (m_threads[i].taskId == taskId) {
            m_threads[i].progress = progress;
            m_threads[i].cpuUsage = 1.5 + (QRandomGenerator::global()->generateDouble() * 4.5);
            QModelIndex idx = createIndex(i, 0);
            emit dataChanged(idx, idx, {ProgressRole, CpuUsageRole});
            break;
        }
    }
}

void ThreadModel::onTaskFinished(const QString &taskId, bool success)
{
    for (int i = 0; i < m_threads.count(); ++i) {
        if (m_threads[i].taskId == taskId) {
            m_threads[i].status = success ? "Completed" : "Cancelled";
            m_threads[i].progress = success ? 100 : m_threads[i].progress;
            m_threads[i].cpuUsage = 0.0;
            QModelIndex idx = createIndex(i, 0);
            emit dataChanged(idx, idx, {StatusRole, ProgressRole, CpuUsageRole});
            break;
        }
    }

    emit activeCountChanged();

    if (activeCount() == 0) {
        emit allTasksFinished();
    }
}

#ifndef UPLOADTASK_H
#define UPLOADTASK_H

#include <QThread>
#include <QString>
#include <atomic>

class UploadTask : public QThread
{
    Q_OBJECT
public:
    UploadTask(const QString &taskId,
               const QString &fileName,
               qint64 fileSizeKb,
               const QString &targetDir,
               QObject *parent = nullptr);

    QString taskId() const;
    QString fileName() const;
    qint64 fileSizeKb() const;
    int progress() const;
    QString speed() const;
    QString status() const;

    void cancel();

signals:
    void progressUpdated(const QString &taskId, int progress, const QString &speed);
    void taskFinished(const QString &taskId, bool success);

protected:
    void run() override;

private:
    QString m_taskId;
    QString m_fileName;
    qint64 m_fileSizeKb;
    QString m_targetDir;
    int m_progress;
    QString m_speed;
    QString m_status;
    std::atomic<bool> m_isCancelled;
};

#endif // UPLOADTASK_H

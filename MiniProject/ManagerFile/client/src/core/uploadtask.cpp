#include "uploadtask.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

UploadTask::UploadTask(const QString &taskId,
                       const QString &fileName,
                       qint64 fileSizeKb,
                       const QString &targetDir,
                       QObject *parent)
    : QThread(parent)
    , m_taskId(taskId)
    , m_fileName(fileName)
    , m_fileSizeKb(fileSizeKb)
    , m_targetDir(targetDir)
    , m_progress(0)
    , m_speed("0 MB/s")
    , m_status("Running")
    , m_isCancelled(false)
{
}

QString UploadTask::taskId() const { return m_taskId; }
QString UploadTask::fileName() const { return m_fileName; }
qint64 UploadTask::fileSizeKb() const { return m_fileSizeKb; }
int UploadTask::progress() const { return m_progress; }
QString UploadTask::speed() const { return m_speed; }
QString UploadTask::status() const { return m_status; }

void UploadTask::cancel()
{
    m_isCancelled = true;
    m_status = "Cancelled";
}

void UploadTask::run()
{
    qInfo() << "[UploadTask" << m_taskId << "] Started uploading:" << m_fileName;

    // 100 steps. For a 2-minute total demo delay, each step sleeps 1200ms (100 * 1.2s = 120s).
    // Note: For user test convenience, if fileSizeKb < 1000, we use faster sleep (100ms) for quick testing,
    // otherwise 1200ms for full 2-minute demo simulation.
    int sleepMs = (m_fileSizeKb > 5000) ? 1200 : 150;

    for (int step = 1; step <= 100; ++step) {
        if (m_isCancelled) {
            qInfo() << "[UploadTask" << m_taskId << "] Cancelled at" << step << "%";
            emit taskFinished(m_taskId, false);
            return;
        }

        QThread::msleep(sleepMs);
        m_progress = step;

        double simulatedSpeedMb = 1.5 + ((step % 7) * 0.2);
        m_speed = QString::number(simulatedSpeedMb, 'f', 1) + " MB/s";

        emit progressUpdated(m_taskId, m_progress, m_speed);
    }

    // Complete upload by writing dummy payload file into cloud_root target dir
    QDir dir(m_targetDir);
    if (!dir.exists()) dir.mkpath(".");
    
    QFile file(dir.filePath(m_fileName));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Simulated uploaded content for file: " << m_fileName << "\n";
        out << "File size: " << m_fileSizeKb << " KB\n";
        file.close();
    }

    m_status = "Completed";
    qInfo() << "[UploadTask" << m_taskId << "] Completed successfully!";
    emit taskFinished(m_taskId, true);
}

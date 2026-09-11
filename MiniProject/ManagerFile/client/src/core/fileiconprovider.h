#ifndef FILEICONPROVIDER_H
#define FILEICONPROVIDER_H

#include <QQuickImageProvider>
#include <QFileIconProvider>

class FileIconProvider : public QQuickImageProvider
{
public:
    FileIconProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QFileIconProvider m_sysIconProvider;
};

#endif // FILEICONPROVIDER_H

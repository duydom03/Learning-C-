#include "fileiconprovider.h"
#include <QPainter>
#include <QIcon>
#include <QFileInfo>
#include <QTemporaryFile>

FileIconProvider::FileIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap FileIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width  = (requestedSize.width() > 0) ? requestedSize.width() : 48;
    int height = (requestedSize.height() > 0) ? requestedSize.height() : 48;
    if (size) *size = QSize(width, height);

    QPixmap pixmap(width, height);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    if (id == "folder") {
        // Draw folder icon
        painter.setBrush(QColor("#4285F4")); // Google Drive blue
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(4, 12, width - 8, height - 16, 6, 6);
        painter.drawRoundedRect(4, 6, width / 2, 10, 4, 4);
    } else {
        // Draw document file icon based on type
        QColor iconColor("#5F6368");
        QString label = "FILE";

        if (id == "document-text") { iconColor = QColor("#4285F4"); label = "TXT"; }
        else if (id == "document-pdf") { iconColor = QColor("#EA4335"); label = "PDF"; }
        else if (id == "document-word") { iconColor = QColor("#1A73E8"); label = "DOC"; }
        else if (id == "document-excel") { iconColor = QColor("#0F9D58"); label = "XLS"; }
        else if (id == "image") { iconColor = QColor("#FA7B17"); label = "IMG"; }
        else if (id == "audio") { iconColor = QColor("#A142F4"); label = "AUD"; }
        else if (id == "video") { iconColor = QColor("#F44236"); label = "VID"; }
        else if (id == "archive") { iconColor = QColor("#F4B400"); label = "ZIP"; }

        // Paper body
        painter.setBrush(iconColor);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(6, 4, width - 12, height - 8, 4, 4);

        // Folded corner
        painter.setBrush(QColor(255, 255, 255, 180));
        QPolygon corner;
        corner << QPoint(width - 16, 4) << QPoint(width - 6, 14) << QPoint(width - 16, 14);
        painter.drawPolygon(corner);

        // Type label text
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        font.setPixelSize(qMax(9, width / 4));
        painter.setFont(font);
        painter.drawText(QRect(6, height / 2 - 4, width - 12, height / 2),
                         Qt::AlignCenter, label);
    }

    return pixmap;
}

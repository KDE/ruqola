/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messageattachmentdelegatehelpervideo.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "dialogs/showvideodialog.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperVideo::~MessageAttachmentDelegateHelperVideo()
{
}

void MessageAttachmentDelegateHelperVideo::draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const VideoLayout layout = layoutVideo(msgAttach, option);
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);

    const QIcon showIcon = QIcon::fromTheme(QStringLiteral("visibility"));
    showIcon.paint(painter, layout.showButtonRect.translated(messageRect.topLeft()));
    const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
    downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();

    // Draw description (if any)
    if (!layout.description.isEmpty()) {
        painter->drawText(messageRect.x(), nextY + option.fontMetrics.ascent(), layout.description);
    }
}

QSize MessageAttachmentDelegateHelperVideo::sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(maxWidth)
    Q_UNUSED(index)
    const VideoLayout layout = layoutVideo(msgAttach, option);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int pixmapWidth = 0;
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return QSize(qMax(qMax(pixmapWidth, layout.titleSize.width()), descriptionWidth),
                 height);
}

bool MessageAttachmentDelegateHelperVideo::handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(index)
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();

        VideoLayout layout = layoutVideo(msgAttach, option);
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            const QString file = DelegateUtil::querySaveFileName(parentWidget, i18n("Save Video"), QUrl::fromLocalFile(layout.videoPath));
            if (!file.isEmpty()) {
                QFile::remove(file); // copy() doesn't overwrite
                QFile sourceFile(layout.videoPath);
                if (!sourceFile.copy(file)) {
                    QMessageBox::warning(parentWidget, i18n("Error saving file"), sourceFile.errorString());
                }
            }
            return true;
        } else if (attachmentsRect.contains(pos) || layout.showButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            QPointer<ShowVideoDialog> dlg = new ShowVideoDialog(parentWidget);
            dlg->setVideoUrl(QUrl::fromLocalFile(layout.videoPath));
            dlg->exec();
            delete dlg;
            return true;
        }
    }
    return false;
}

MessageAttachmentDelegateHelperVideo::VideoLayout MessageAttachmentDelegateHelperVideo::layoutVideo(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option) const
{
    VideoLayout layout;
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    if (url.isLocalFile()) {
        layout.videoPath = url.toLocalFile();
        //or we could do layout.attachment = msgAttach; if we need many fields from it
        layout.title = msgAttach.title();
        layout.description = msgAttach.description();
        layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
        layout.descriptionSize = layout.description.isEmpty() ? QSize(0, 0) : option.fontMetrics.size(Qt::TextSingleLine, layout.description);
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.showButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
        layout.downloadButtonRect = layout.showButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
    }
    return layout;
}

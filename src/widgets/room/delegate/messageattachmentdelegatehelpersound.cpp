/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "messageattachmentdelegatehelpersound.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "dialogs/playsounddialog.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperSound::MessageAttachmentDelegateHelperSound()
    : mPlayerVolumeIcon(QIcon::fromTheme(QStringLiteral("player-volume")))
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
{
}

MessageAttachmentDelegateHelperSound::~MessageAttachmentDelegateHelperSound()
{
}

void MessageAttachmentDelegateHelperSound::draw(const MessageAttachment &msgAttach,
                                                QPainter *painter,
                                                QRect messageRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const SoundLayout layout = layoutSound(msgAttach, option, messageRect.width());
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    mPlayerVolumeIcon.paint(painter, layout.playerVolumeButtonRect.translated(messageRect.topLeft()));
    mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
    drawDescription(msgAttach, messageRect, painter, nextY);
}

QSize MessageAttachmentDelegateHelperSound::sizeHint(const MessageAttachment &msgAttach,
                                                     const QModelIndex &index,
                                                     int maxWidth,
                                                     const QStyleOptionViewItem &option) const
{
    Q_UNUSED(maxWidth)
    Q_UNUSED(index)
    const SoundLayout layout = layoutSound(msgAttach, option, maxWidth);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    const int pixmapWidth = 0;
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(pixmapWidth, layout.titleSize.width()), descriptionWidth), height};
}

bool MessageAttachmentDelegateHelperSound::handleMouseEvent(const MessageAttachment &msgAttach,
                                                            QMouseEvent *mouseEvent,
                                                            QRect attachmentsRect,
                                                            const QStyleOptionViewItem &option,
                                                            const QModelIndex &index)
{
    Q_UNUSED(index)
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();

        const SoundLayout layout = layoutSound(msgAttach, option, attachmentsRect.width());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto *parentWidget = const_cast<QWidget *>(option.widget);
            const QString file = DelegateUtil::querySaveFileName(parentWidget, i18n("Save Sound"), QUrl::fromLocalFile(layout.audioPath));
            if (!file.isEmpty()) {
                QFile::remove(file); // copy() doesn't overwrite
                QFile sourceFile(layout.audioPath);
                if (!sourceFile.copy(file)) {
                    QMessageBox::warning(parentWidget, i18n("Error saving file"), sourceFile.errorString());
                }
            }
            return true;
        } else if (attachmentsRect.contains(pos) || layout.playerVolumeButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto *parentWidget = const_cast<QWidget *>(option.widget);
            PlaySoundDialog dlg(parentWidget);
            dlg.setAudioUrl(QUrl::fromLocalFile(layout.audioPath));
            dlg.exec();
            return true;
        }
    }
    return false;
}

MessageAttachmentDelegateHelperSound::SoundLayout
MessageAttachmentDelegateHelperSound::layoutSound(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    SoundLayout layout;
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    // or we could do layout.attachment = msgAttach; if we need many fields from it
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.playerVolumeButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    layout.downloadButtonRect = layout.playerVolumeButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
    if (url.isLocalFile()) {
        layout.audioPath = url.toLocalFile();
    }
    return layout;
}

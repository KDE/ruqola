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

#include "messagedelegatehelpersound.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "common/delegatepaintutil.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyleOptionViewItem>

void MessageDelegateHelperSound::draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const SoundLayout layout = layoutSound(message, option, messageRect.width(), messageRect.height());
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    const QIcon hideShowIcon = QIcon::fromTheme(QStringLiteral("visibility"));
    hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(messageRect.topLeft()));
    const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
    downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    // Draw main pixmap (if shown)
    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();
    // Draw description (if any)
    if (!layout.description.isEmpty()) {
        painter->drawText(messageRect.x(), nextY + option.fontMetrics.ascent(), layout.description);
    }
}

QSize MessageDelegateHelperSound::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const SoundLayout layout = layoutSound(message, option, maxWidth, -1);
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

bool MessageDelegateHelperSound::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        const QPoint pos = mouseEvent->pos();

        SoundLayout layout = layoutSound(message, option, attachmentsRect.width(), attachmentsRect.height());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            const QString file = QFileDialog::getSaveFileName(parentWidget, i18n("Save Sound"));
            if (!file.isEmpty()) {
                QFile::remove(file); // copy() doesn't overwrite
                QFile sourceFile(layout.imagePath);
                if (!sourceFile.copy(file)) {
                    QMessageBox::warning(parentWidget, i18n("Error saving file"), sourceFile.errorString());
                }
            }
            return true;
        } else if (attachmentsRect.contains(pos) || layout.hideShowButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            //                QPointer<ShowVideoDialog> dlg = new ShowVideoDialog(parentWidget);
            //                dlg->setVideoUrl(QUrl::fromLocalFile(layout.imagePath)); //FIX me
            //                dlg->exec();
            //                delete dlg;
            return true;
        }
    }
    return false;
}

MessageDelegateHelperSound::SoundLayout MessageDelegateHelperSound::layoutSound(const Message *message, const QStyleOptionViewItem &option, int attachmentsWidth, int attachmentsHeight) const
{
    SoundLayout layout;
    if (message->attachements().isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "No attachments in Sound message";
        return layout;
    }
    if (message->attachements().count() > 1) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Multiple attachments in Sound message? Can this happen?";
    }
    const MessageAttachment &msgAttach = message->attachements().at(0);
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    if (url.isLocalFile()) {
        layout.imagePath = url.toLocalFile();
        //or we could do layout.attachment = msgAttach; if we need many fields from it
        layout.title = msgAttach.title();
        layout.description = msgAttach.description();
        layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
        layout.descriptionSize = layout.description.isEmpty() ? QSize(0, 0) : option.fontMetrics.size(Qt::TextSingleLine, layout.description);
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.hideShowButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
        layout.downloadButtonRect = layout.hideShowButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);

        if (attachmentsHeight > 0) {
            // Vertically: attachmentsHeight = title | DelegatePaintUtil::margin() | image | DelegatePaintUtil::margin() [| description | DelegatePaintUtil::margin()]
            int imageMaxHeight = attachmentsHeight - layout.titleSize.height() - DelegatePaintUtil::margin() * 2;
            if (!layout.description.isEmpty()) {
                imageMaxHeight -= layout.descriptionSize.height() + DelegatePaintUtil::margin();
            }
        }
    }
    return layout;
}

/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagedelegatehelperimage.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"

#include <QPainter>
#include <QPixmapCache>

void MessageDelegateHelperImage::draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, qreal *) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    ImageLayout layout = layoutImage(message);
    if (!layout.pixmap.isNull()) {
        painter->drawPixmap(messageRect.topLeft(), layout.pixmap.scaled(messageRect.size(), Qt::KeepAspectRatio));
        // TODO show layout.title
    }
}

QSize MessageDelegateHelperImage::sizeHint(const QModelIndex &index, int maxWidth) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const ImageLayout layout = layoutImage(message);
    const int idealWidth = qMin(layout.pixmap.width(), maxWidth);
    const int height = qMin(layout.pixmap.height(), 200);
    return QSize(idealWidth, height);
}

MessageDelegateHelperImage::ImageLayout MessageDelegateHelperImage::layoutImage(const Message *message) const
{
    ImageLayout layout;
    if (message->attachements().isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "No attachments in Image message";
        return layout;
    }
    if (message->attachements().count() > 1) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Multiple attachments in Image message? Can this happen?";
    }
    const MessageAttachment &msgAttach = message->attachements().at(0);
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    if (url.isLocalFile()) {
        const QString path = url.toLocalFile();
        QPixmap pixmap;
        if (!QPixmapCache::find(path, &pixmap)) {
            if (pixmap.load(path)) {
                QPixmapCache::insert(path, pixmap);
            } else {
                qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << path;
            }
        }
        layout.pixmap = pixmap;
        layout.title = msgAttach.title();
        //or we could do layout.attachment = msgAttach;
    }
    return layout;
}

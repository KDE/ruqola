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
#include <QStyleOptionViewItem>

void MessageDelegateHelperImage::draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, const QStyleOptionViewItem &option, qreal *) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    ImageLayout layout = layoutImage(message);
    if (!layout.pixmap.isNull()) {
        const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
        const QSize descriptionSize = layout.description.isEmpty() ? QSize(0, 0) : option.fontMetrics.size(Qt::TextSingleLine, layout.description);
        const QSize imageSize(messageRect.width(), messageRect.height() - descriptionSize.height() - titleSize.height());

        const QPixmap scaledPixmap = layout.pixmap.scaled(imageSize, Qt::KeepAspectRatio);
        painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
        painter->drawPixmap(messageRect.x(), messageRect.y() + titleSize.height(), scaledPixmap);
        if (!layout.description.isEmpty()) {
            painter->drawText(messageRect.x(), messageRect.y() + titleSize.height() + scaledPixmap.height() + option.fontMetrics.ascent(), layout.description);
        }
    }
}

QSize MessageDelegateHelperImage::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const ImageLayout layout = layoutImage(message);
    const int idealWidth = qMin(layout.pixmap.width(), maxWidth);
    const int height = qMin(layout.pixmap.height(), 200);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    const QSize descriptionSize = layout.description.isEmpty() ? QSize(0, 0) : option.fontMetrics.size(Qt::TextSingleLine, layout.description);
    return QSize(qMax(qMax(idealWidth, titleSize.width()), descriptionSize.width()),
                 descriptionSize.height() + height + titleSize.height());
}

bool MessageDelegateHelperImage::handleMouseEvent(QMouseEvent *mouseEvent, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(mouseEvent)
    Q_UNUSED(option)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    ImageLayout layout = layoutImage(message);
    if (!layout.pixmap.isNull()) {
        qDebug() << "Clicked!";
        // TODO use ShowImageDialog or ShowImageWidget here, to show layout.pixmap
        return true;
    }
    return false;
}

QPixmap MessageDelegateHelperImage::findCachedPixmap(const QString &link) const
{
    auto matchesLink = [&](const CachedImage &cached) { return cached.link == link; };
    auto it = std::find_if(m_cachedImages.begin(), m_cachedImages.end(), matchesLink);
    if (it == m_cachedImages.end())
        return QPixmap();
    // Move it to the front
    if (it != m_cachedImages.begin()) {
        const auto idx = std::distance(m_cachedImages.begin(), it);
        m_cachedImages.move(idx, 0);
    }
    return it->pixmap;
}

void MessageDelegateHelperImage::insertCachedPixmap(const QString &link, const QPixmap &pixmap) const
{
    m_cachedImages.prepend(CachedImage{link, pixmap});
    static const int s_maxCacheSize = 5;
    if (m_cachedImages.size() > s_maxCacheSize) {
        m_cachedImages.resize(s_maxCacheSize);
    }
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
        // QPixmapCache is too small for this, let's have our own LRU cache
        QPixmap pixmap = findCachedPixmap(path);
        if (pixmap.isNull()) {
            if (pixmap.load(path)) {
                insertCachedPixmap(path, pixmap);
            } else {
                qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << path;
            }
        }
        layout.pixmap = pixmap;
        layout.title = msgAttach.title();
        layout.description = msgAttach.description();
        //or we could do layout.attachment = msgAttach;
    }
    return layout;
}

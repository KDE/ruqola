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

#include "messagedelegatehelperfile.h"
#include "model/messagemodel.h"
#include "ruqolawidgets_debug.h"

#include <QPainter>
#include <QStyleOptionViewItem>

//  Name <download icon>
//  Description

static const int vMargin = 8;

void MessageDelegateHelperFile::draw(QPainter *painter, const QRect &rect, const QModelIndex &index, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(pBaseLine)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const FileInfo info = gatherInfo(message, option);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, info.title);
    //const QSize descriptionSize = option.fontMetrics.size(Qt::TextSingleLine, info.description);

    painter->drawText(rect.x(), rect.y() + option.fontMetrics.ascent(), info.title);
    const int nextY = rect.y() + titleSize.height() + vMargin;
    painter->drawText(rect.x(), nextY + option.fontMetrics.ascent(), info.description);
}

QSize MessageDelegateHelperFile::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(maxWidth)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const FileInfo info = gatherInfo(message, option);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, info.title);
    const QSize descriptionSize = option.fontMetrics.size(Qt::TextSingleLine, info.description);
    return QSize(qMax(titleSize.width(), descriptionSize.width()),
                 titleSize.height() + vMargin + descriptionSize.height());
}

MessageDelegateHelperFile::FileInfo MessageDelegateHelperFile::gatherInfo(const Message *message, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(option);

    FileInfo info;
    if (message->attachements().isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "No attachments in File message";
        return info;
    }
    if (message->attachements().count() > 1) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Multiple attachments in File message? Can this happen?";
    }
    const MessageAttachment &msgAttach = message->attachements().at(0);
    //const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    info.title = msgAttach.title();
    info.description = msgAttach.description();
    return info;
}

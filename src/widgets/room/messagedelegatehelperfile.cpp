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

void MessageDelegateHelperFile::draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(pBaseLine)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const FileLayout layout = doLayout(message, option);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);

    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
    downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    const int nextY = messageRect.y() + titleSize.height() + vMargin;
    painter->drawText(messageRect.x(), nextY + option.fontMetrics.ascent(), layout.description);
}

QSize MessageDelegateHelperFile::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(maxWidth)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const FileLayout info = doLayout(message, option);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, info.title);
    const QSize descriptionSize = option.fontMetrics.size(Qt::TextSingleLine, info.description);
    return QSize(qMax(titleSize.width(), descriptionSize.width()),
                 titleSize.height() + vMargin + descriptionSize.height());
}

MessageDelegateHelperFile::FileLayout MessageDelegateHelperFile::doLayout(const Message *message, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(option);

    FileLayout layout;
    if (message->attachements().isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "No attachments in File message";
        return layout;
    }
    if (message->attachements().count() > 1) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Multiple attachments in File message? Can this happen?";
    }
    const MessageAttachment &msgAttach = message->attachements().at(0);
    //const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrl(msgAttach.link());
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();

    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    const int buttonMargin = 8;
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.downloadButtonRect = QRect(layout.titleSize.width() + buttonMargin, 0, iconSize, iconSize);

    return layout;
}

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

#include "channellistdelegate.h"

#include "model/roommodel.h"

#include <QPainter>

ChannelListDelegate::ChannelListDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void ChannelListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <icon> [M] <name>       <(nr_unread)> [M]    ([M] = margin)
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const int margin = 8;
    const QRect decorationRect(option.rect.x() + margin, option.rect.y(), iconSize, option.rect.height());
    const QString text = index.data(Qt::DisplayRole).toString();
    //const QSize textSize = option.fontMetrics.size(Qt::TextSingleLine, text);
    const int unreadCount = index.data(RoomModel::RoomUnread).toInt();
    const QString unreadText = unreadCount > 0 ? QStringLiteral("(%1)").arg(unreadCount) : QString();
    const QSize unreadSize = unreadCount > 0 ? option.fontMetrics.size(Qt::TextSingleLine, unreadText) : QSize(0, 0);
    const int xText = option.rect.x() + iconSize + 2 * margin;
    const QRect displayRect(xText, option.rect.y(),
                            option.rect.width() - xText - unreadSize.width() - margin,
                            option.rect.height());
    const QRect unreadRect(option.rect.width() - unreadSize.width() - margin,
                           option.rect.y(), unreadSize.width(), option.rect.height());

    drawBackground(painter, option, index);
    const QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    icon.paint(painter, decorationRect, Qt::AlignCenter);

    QStyleOptionViewItem optionCopy = option;
    if (unreadCount == 0)
        optionCopy.state &= ~QStyle::State_Enabled;
    drawDisplay(painter, optionCopy, displayRect, text); // this takes care of eliding if the text is too long

    painter->setPen(Qt::red);
    painter->drawText(unreadRect, unreadText);
}

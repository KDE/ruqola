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
#include "common/delegatepaintutil.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "misc/avatarcachemanager.h"
#include "ruqolaglobalconfig.h"

#include <QPainter>
#include <KColorScheme>


ChannelListDelegate::ChannelListDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
{
}

ChannelListDelegate::~ChannelListDelegate()
{
}

void ChannelListDelegate::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(currentRocketChatAccount);
    mRocketChatAccount = currentRocketChatAccount;
}

void ChannelListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    // [M] <avatar> [M] <icon> [M] <name>       <(nr_unread)> [M]    ([M] = margin)
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const int margin = DelegatePaintUtil::margin();
    int offsetAvatarRoom = 0;
    if (RuqolaGlobalConfig::self()->showRoomAvatar()) {
        offsetAvatarRoom = margin + iconSize;
    }
    const QRect decorationRect(option.rect.x() + margin + offsetAvatarRoom, option.rect.y(), iconSize, option.rect.height());
    const QString text = index.data(Qt::DisplayRole).toString();
    //const QSize textSize = option.fontMetrics.size(Qt::TextSingleLine, text);
    const QString unreadText = makeUnreadText(index);
    const QSize unreadSize = !unreadText.isEmpty() ? option.fontMetrics.size(Qt::TextSingleLine, unreadText) : QSize(0, 0);
    const int xText = offsetAvatarRoom + option.rect.x() + iconSize + 2 * margin;
    const QRect displayRect(xText, option.rect.y(),
                            option.rect.width() - xText - unreadSize.width() - margin,
                            option.rect.height());
    const QRect unreadRect(option.rect.width() - unreadSize.width() - margin,
                           option.rect.y(), unreadSize.width(), option.rect.height());

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;
    if (unreadText.isEmpty() && !index.data(RoomModel::RoomAlert).toBool()) {
        optionCopy.state &= ~QStyle::State_Enabled;
    }
    drawBackground(painter, optionCopy, index);
    const QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    icon.paint(painter, decorationRect, Qt::AlignCenter);
    if (RuqolaGlobalConfig::self()->showRoomAvatar()) {
        const QString avatarUrl = index.data(RoomModel::RoomAvatar).toString();
        if (!avatarUrl.isEmpty()) {
            const QString roomId = index.data(RoomModel::RoomId).toString();
            const QPixmap pix = mAvatarCacheManager->makeAvatarUrlPixmap(option.widget, roomId, option.rect.height());
            painter->drawPixmap(option.rect.topLeft(), pix);
            //qDebug() << "avatarUrl " << avatarUrl;
        }
    }

    drawDisplay(painter, optionCopy, displayRect, text); // this takes care of eliding if the text is too long

    KColorScheme scheme;
    painter->setPen(scheme.foreground(KColorScheme::NegativeText).color());
    painter->drawText(unreadRect, unreadText);
}

QString ChannelListDelegate::makeUnreadText(const QModelIndex &index) const
{
    const int unreadCount = index.data(RoomModel::RoomUnread).toInt();
    const QString unreadText = unreadCount > 0 ? QStringLiteral("(%1)").arg(unreadCount) : QString();
    const int userMentionsCount = index.data(RoomModel::RoomUserMentions).toInt();
    return (userMentionsCount > 0) ? QLatin1Char('@') + unreadText : unreadText;
}

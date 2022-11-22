/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistdelegate.h"
#include "colors.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"

#include <KColorScheme>
#include <QPainter>

namespace
{
constexpr uint padding = 2;
}

ChannelListDelegate::ChannelListDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
{
}

ChannelListDelegate::~ChannelListDelegate() = default;

void ChannelListDelegate::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(currentRocketChatAccount);
    mRocketChatAccount = currentRocketChatAccount;
}

void ChannelListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <avatar> [M] <icon> [M] <name>       <(nr_unread)> [M]    ([M] = margin)
    const auto isHeader = !index.parent().isValid();
    const int iconSize = isHeader ? 0 : option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const int margin = DelegatePaintUtil::margin();
    int offsetAvatarRoom = 0;
    const bool showRoomAvatar = mRocketChatAccount ? mRocketChatAccount->ownUserPreferences().showRoomAvatar() : false;
    if (!isHeader && showRoomAvatar) {
        offsetAvatarRoom = margin + option.rect.height() - 2 * padding; // Icon will be draw with option.rect.height() - 2 * padding as size.
    }
    const QRect decorationRect(option.rect.x() + margin + offsetAvatarRoom, option.rect.y() + padding, iconSize, option.rect.height() - 2 * padding);
    const QString text = index.data(Qt::DisplayRole).toString();
    // const QSize textSize = option.fontMetrics.size(Qt::TextSingleLine, text);
    const QString unreadText = isHeader ? QString() : makeUnreadText(index);
    const QSize unreadSize = !unreadText.isEmpty() ? option.fontMetrics.size(Qt::TextSingleLine, unreadText) : QSize(0, 0);
    const int xText = offsetAvatarRoom + option.rect.x() + iconSize + (isHeader ? 1 : 2) * margin;
    const QRect displayRect(xText, option.rect.y() + padding, option.rect.width() - xText - unreadSize.width() - margin, option.rect.height() - 2 * padding);
    const QRect unreadRect(option.rect.width() - unreadSize.width() - margin,
                           option.rect.y() + padding,
                           unreadSize.width(),
                           option.rect.height() - 2 * padding);

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;
    drawBackground(painter, optionCopy, index);

    if (!isHeader) {
        const auto icon = index.data(Qt::DecorationRole).value<QIcon>();
        icon.paint(painter, decorationRect, Qt::AlignCenter);
        if (showRoomAvatar) {
            const auto avatarInfo = index.data(RoomModel::RoomAvatarInfo).value<Utils::AvatarInfo>();
            if (avatarInfo.isValid()) {
                const QPixmap pix = mAvatarCacheManager->makeAvatarUrlPixmap(option.widget, avatarInfo, option.rect.height() - 2 * padding);
                painter->drawPixmap(margin, option.rect.top() + padding, pix);
            }
        }
    }

    if (!(unreadText.isEmpty() && !index.data(RoomModel::RoomAlert).toBool())) {
        if (!index.data(RoomModel::HideBadgeForMention).toBool()) {
            optionCopy.palette.setBrush(QPalette::Text, optionCopy.palette.brush(QPalette::Link));
            if (option.state & QStyle::State_Selected) {
                optionCopy.palette.setBrush(QPalette::Text, optionCopy.palette.brush(QPalette::HighlightedText));
            }
        }
    } else {
        if (option.state & QStyle::State_Selected) {
            optionCopy.palette.setBrush(QPalette::Text, optionCopy.palette.brush(QPalette::LinkVisited));
        }
        if (index.data(RoomModel::UserOffline).toBool()) {
            optionCopy.palette.setBrush(QPalette::Text, Colors::self().schemeView().foreground(KColorScheme::InactiveText).color());
        }
    }
    drawDisplay(painter, optionCopy, displayRect, text); // this takes care of eliding if the text is too long
    if (!isHeader) {
        painter->setPen(Colors::self().schemeView().foreground(KColorScheme::NegativeText).color());
        painter->drawText(unreadRect, unreadText);
    }
}

QString ChannelListDelegate::makeUnreadText(const QModelIndex &index) const
{
    const bool hideBadgeForMention = index.data(RoomModel::HideBadgeForMention).toBool();
    if (hideBadgeForMention) {
        return QString();
    }
    const int unreadCount = index.data(RoomModel::RoomUnread).toInt();
    const QString unreadText = unreadCount > 0 ? QStringLiteral("(%1)").arg(unreadCount) : QString();
    const int userMentionsCount = index.data(RoomModel::RoomUserMentions).toInt();
    return (userMentionsCount > 0) ? QLatin1Char('@') + unreadText : unreadText;
}

QSize ChannelListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index) + QSize(0, 2 * padding);
}

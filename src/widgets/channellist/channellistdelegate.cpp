/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistdelegate.h"
#include "colorsandmessageviewstyle.h"
#include "common/delegatepaintutil.h"
#include "config-ruqola.h"
#include "misc/avatarcachemanager.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"

#include <KColorScheme>
#include <QAbstractItemView>
#include <QHelpEvent>
#include <QPainter>
#include <QToolTip>

namespace
{
constexpr uint padding = 2;
constexpr int extraMargins = 2 * padding;
}

ChannelListDelegate::ChannelListDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
{
    connect(&ColorsAndMessageViewStyle::self(), &ColorsAndMessageViewStyle::needUpdateFontSize, this, &ChannelListDelegate::clearAvatarCache);
}

ChannelListDelegate::~ChannelListDelegate() = default;

void ChannelListDelegate::clearAvatarCache()
{
    mAvatarCacheManager->clearCache();
}

void ChannelListDelegate::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(currentRocketChatAccount);
    mRocketChatAccount = currentRocketChatAccount;
}

void ChannelListDelegate::setListDisplay(OwnUserPreferences::RoomListDisplay display)
{
    if (mRoomListDisplay != display) {
        mRoomListDisplay = display;
        clearAvatarCache();
    }
}

ChannelListDelegate::Layout ChannelListDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ChannelListDelegate::Layout layout;
    layout.isHeader = !index.parent().isValid();
    layout.unreadText = layout.isHeader ? QString() : makeUnreadText(index);
    const int margin = DelegatePaintUtil::margin();
    layout.unreadSize = !layout.unreadText.isEmpty() ? option.fontMetrics.size(Qt::TextSingleLine, layout.unreadText) : QSize(0, 0);
    layout.unreadRect = QRect(option.rect.width() - layout.unreadSize.width() - 2 * margin,
                              option.rect.y() + padding,
                              layout.unreadSize.width() + margin,
                              option.rect.height() - extraMargins);

    return layout;
}

bool ChannelListDelegate::helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!helpEvent || !view || !index.isValid() || !index.parent().isValid() /* header*/) {
        return QItemDelegate::helpEvent(helpEvent, view, option, index);
    }

    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }
    const ChannelListDelegate::Layout layout = doLayout(option, index);
    const QPoint helpEventPos{helpEvent->pos()};
    if (layout.unreadRect.contains(helpEventPos)) {
        const QString unreadToolTip = index.data(RoomModel::RoomUnreadToolTip).toString();
        if (!unreadToolTip.isEmpty()) {
            QToolTip::showText(helpEvent->globalPos(), unreadToolTip, view);
            return true;
        }
    }

    const QString toolTip = index.data(Qt::ToolTipRole).toString();
    if (!toolTip.isEmpty()) {
        QToolTip::showText(helpEvent->globalPos(), toolTip, view);
        return true;
    }
    return true;
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
        offsetAvatarRoom = margin + option.rect.height() - extraMargins; // Icon will be draw with option.rect.height() - 2 * padding as size.
    }

    const QRect decorationRect(option.rect.x() + margin + offsetAvatarRoom, option.rect.y() + padding, iconSize, option.rect.height() - extraMargins);
    const QString text = index.data(Qt::DisplayRole).toString();

    const ChannelListDelegate::Layout layout = doLayout(option, index);

    const int xText = offsetAvatarRoom + option.rect.x() + iconSize + (isHeader ? 1 : 2) * margin;
    const QRect displayRect(xText,
                            option.rect.y() + padding,
                            option.rect.width() - xText - layout.unreadSize.width() - margin,
                            option.rect.height() - extraMargins);

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;
    drawBackground(painter, optionCopy, index);

    if (!isHeader) {
        const auto icon = index.data(Qt::DecorationRole).value<QIcon>();
        icon.paint(painter, decorationRect, Qt::AlignCenter);
        if (showRoomAvatar) {
            const auto avatarInfo = index.data(RoomModel::RoomAvatarInfo).value<Utils::AvatarInfo>();
            if (avatarInfo.isValid()) {
                const QPixmap pix = mAvatarCacheManager->makeAvatarPixmap(option.widget, avatarInfo, option.rect.height() - extraMargins);
#if USE_ROUNDED_RECT_PIXMAP
                const QPointF pos(margin, option.rect.top() + padding);
                DelegatePaintUtil::createClipRoundedRectangle(painter, QRectF(pos, pix.size()), pos, pix);
#else
                painter->drawPixmap(margin, option.rect.top() + padding, pix);
#endif
            }
        }
    }

    if (!(layout.unreadText.isEmpty() && !index.data(RoomModel::RoomAlert).toBool())) {
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
            optionCopy.palette.setBrush(QPalette::Text, ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::InactiveText).color());
        }
    }
    drawDisplay(painter, optionCopy, displayRect, text); // this takes care of eliding if the text is too long
    if (!isHeader && !layout.unreadText.isEmpty()) {
        const RoomModel::MentionsInfoType mentionInfoType = index.data(RoomModel::RoomMentionsInfoType).value<RoomModel::MentionsInfoType>();
        switch (mentionInfoType) {
        case RoomModel::MentionsInfoType::Important:
            painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color());
            break;
        case RoomModel::MentionsInfoType::Warning:
            painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NeutralText).color());
            break;
        case RoomModel::MentionsInfoType::Information:
            painter->setBrush(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::PositiveText).color());
            break;
        case RoomModel::MentionsInfoType::Normal:
            break;
        }
        QRect mentionRect =
            QRect(layout.unreadRect.x(), layout.unreadRect.y(), qMax(layout.unreadRect.width(), layout.unreadRect.height()), layout.unreadRect.height());
        mentionRect.moveRight(layout.unreadRect.right());
        painter->setPen(Qt::NoPen);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawEllipse(mentionRect);
        painter->setPen(ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NormalText).color());
        painter->drawText(layout.unreadRect, Qt::AlignCenter, layout.unreadText);
    }
}

QString ChannelListDelegate::makeUnreadText(const QModelIndex &index) const
{
    const bool hideBadgeForMention = index.data(RoomModel::HideBadgeForMention).toBool();
    if (hideBadgeForMention) {
        return QString();
    }
    const int unreadCount = index.data(RoomModel::RoomUnread).toInt() + index.data(RoomModel::RoomThreadUnread).toInt();
    const QString unreadText = unreadCount > 0 ? QString::number(unreadCount) : QString();
    return unreadText;
}

QSize ChannelListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const auto isHeader = !index.parent().isValid();
    int height = 0;
    const QSize size = QItemDelegate::sizeHint(option, index);
    switch (mRoomListDisplay) {
    case OwnUserPreferences::RoomListDisplay::Unknown:
        break;
    case OwnUserPreferences::RoomListDisplay::Condensed:
        height = 0;
        break;
    case OwnUserPreferences::RoomListDisplay::Medium:
        height = size.height();
        break;
    case OwnUserPreferences::RoomListDisplay::Extended:
        height = size.height() * 1.5;
        break;
    }
    return size + QSize(0, (isHeader ? 0 : height) + extraMargins);
}

#include "moc_channellistdelegate.cpp"

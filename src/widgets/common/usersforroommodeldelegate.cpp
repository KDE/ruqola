/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroommodeldelegate.h"

#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/usersforroommodel.h"

#include <QAbstractItemView>
#include <QHelpEvent>
#include <QPainter>
#include <QToolTip>

using namespace Qt::Literals::StringLiterals;
UsersForRoomModelDelegate::UsersForRoomModelDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
{
}

UsersForRoomModelDelegate::~UsersForRoomModelDelegate() = default;

void UsersForRoomModelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Section rows are top-level rows in tree mode and should use default
    // painting to show their DisplayRole title (Online, Offline, ...).
    // We cannot rely on hasChildren() because empty sections would be missed.
    if (!index.parent().isValid()) {
        QItemDelegate::paint(painter, option, index);
        return;
    }

    // [M] icon ? status name (username)
    drawBackground(painter, option, index);

    if (!option.showDecorationSelected && (option.state & QStyle::State_Selected)) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    const QFontMetrics fontMetrics(boldFont);
    const int iconSize = option.rect.height() - 4;
    const int iconY = option.rect.y() + 2;

    int xPos = 0;
    const Utils::AvatarInfo info = index.data(UsersForRoomModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            painter->drawPixmap(margin, iconY, iconSize, iconSize, pix);
        }
        // Add extra size even if we don't have avatar pix
        xPos = margin + iconSize;
    }

    const QString iconStatusStr = index.data(UsersForRoomModel::IconStatus).toString();
    if (!iconStatusStr.isEmpty()) {
        const QIcon iconStatus = QIcon::fromTheme(iconStatusStr);
        painter->drawPixmap(margin + xPos, iconY, iconSize, iconSize, iconStatus.pixmap(iconSize, iconSize));
        xPos += margin + iconSize;
    }

    const QString name = index.data(UsersForRoomModel::Name).toString();
    const QString userName = index.data(UsersForRoomModel::UserName).toString();
    const int defaultCharHeight = option.rect.y() + (option.rect.height() - fontMetrics.height()) / 2 + fontMetrics.ascent();
    if (name.isEmpty()) {
        painter->drawText(xPos + margin, defaultCharHeight, userName);
    } else {
        const int nameWidth = fontMetrics.horizontalAdvance(name);
        painter->drawText(xPos + margin, defaultCharHeight, name);
        xPos += nameWidth;
        if (!userName.isEmpty()) {
            painter->setFont(oldFont);
            DelegatePaintUtil::drawLighterText(painter, u"(%1)"_s.arg(userName), QPoint(xPos + margin * 2, defaultCharHeight));
        }
    }
    painter->setFont(oldFont);
}

void UsersForRoomModelDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize UsersForRoomModelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4);
}

bool UsersForRoomModelDelegate::helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &, const QModelIndex &index)
{
    if (helpEvent->type() == QEvent::ToolTip) {
        const QStringList roles = index.data(UsersForRoomModel::Roles).toStringList();
        if (!roles.isEmpty()) {
            QToolTip::showText(helpEvent->globalPos(), roles.join(u','), view);
            return true;
        }
    }
    return false;
}

#include "moc_usersforroommodeldelegate.cpp"

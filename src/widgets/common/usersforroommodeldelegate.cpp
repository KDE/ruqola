/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroommodeldelegate.h"
using namespace Qt::Literals::StringLiterals;

#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/usersforroommodel.h"

#include <QAbstractItemView>
#include <QHelpEvent>
#include <QPainter>
#include <QToolTip>

UsersForRoomModelDelegate::UsersForRoomModelDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
{
}

UsersForRoomModelDelegate::~UsersForRoomModelDelegate() = default;

void UsersForRoomModelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? status name (username)
    drawBackground(painter, option, index);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    int xPos = -1;
    const Utils::AvatarInfo info = index.data(UsersForRoomModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, option.rect.height());
        if (!pix.isNull()) {
            drawDecoration(painter, option, displayRect, pix);
        }
        // Add extra size even if we don't have avatar pix
        xPos = margin + option.rect.height();
    }

    const QString iconStatusStr = index.data(UsersForRoomModel::IconStatus).value<QString>();
    if (!iconStatusStr.isEmpty()) {
        const QIcon iconStatus = QIcon::fromTheme(iconStatusStr);
        const QRect displayRect(margin + xPos, option.rect.y(), option.rect.height(), option.rect.height());
        drawDecoration(painter, option, displayRect, iconStatus.pixmap(option.rect.height(), option.rect.height()));
        xPos += margin + option.rect.height();
    }

    QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(UsersForRoomModel::Name).toString();
    const QString userName = index.data(UsersForRoomModel::UserName).toString();
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    if (name.isEmpty()) {
        painter->drawText(xPos + margin, defaultCharHeight, userName);
    } else {
        int nameWidth = fontMetrics.horizontalAdvance(name);
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
    return size + QSize(0, 4 * option.widget->devicePixelRatioF());
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

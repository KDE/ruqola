/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroommodeldelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/usersforroommodel.h"

#include <QPainter>

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
            xPos = margin + option.rect.height();
        }
    }

    const QIcon iconStatus = index.data(UsersForRoomModel::IconStatus).value<QIcon>();
    if (!iconStatus.isNull()) {
        const QRect displayRect(margin + xPos, option.rect.y(), option.rect.height(), option.rect.height());
        drawDecoration(painter, option, displayRect, iconStatus.pixmap(option.rect.height(), option.rect.height()));
        xPos += margin + option.rect.height();
    }

    QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(UsersForRoomModel::DisplayName).toString();
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
            DelegatePaintUtil::drawLighterText(painter, QStringLiteral("(%1)").arg(userName), QPoint(xPos + margin * 2, defaultCharHeight));
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

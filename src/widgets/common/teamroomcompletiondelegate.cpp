/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamroomcompletiondelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/teamroomcompletermodel.h"

#include <QPainter>

TeamRoomCompletionDelegate::TeamRoomCompletionDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
{
}

TeamRoomCompletionDelegate::~TeamRoomCompletionDelegate() = default;

void TeamRoomCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon name
    drawBackground(painter, option, index);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    const QFontMetrics fontMetrics(boldFont);
    const int defaultCharHeight = option.rect.y() + (option.rect.height() - fontMetrics.height()) / 2 + fontMetrics.ascent();

    int xPos = 0;
    const Utils::AvatarInfo info = index.data(TeamRoomCompleterModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const int iconSize = option.rect.height() - 4;
        const int iconY = option.rect.y() + 2;
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            painter->drawPixmap(margin, iconY, iconSize, iconSize, pix);
        }
        xPos = margin + iconSize;
    }

    const QString name = index.data(TeamRoomCompleterModel::TeamName).toString();
    painter->drawText(xPos + margin, defaultCharHeight, name);
    painter->setFont(oldFont);
}

void TeamRoomCompletionDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize TeamRoomCompletionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4);
}

#include "moc_teamroomcompletiondelegate.cpp"

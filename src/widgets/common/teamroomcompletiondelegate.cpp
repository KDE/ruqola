/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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

    int xPos = -1;
    QFontMetrics fontMetrics(boldFont);
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    const Utils::AvatarInfo info = index.data(TeamRoomCompleterModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, option.rect.height());
        if (!pix.isNull()) {
            drawDecoration(painter, option, displayRect, pix);
        }
        xPos = margin + option.rect.height();
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
    return size + QSize(0, 4 * option.widget->devicePixelRatioF());
}

#include "moc_teamroomcompletiondelegate.cpp"

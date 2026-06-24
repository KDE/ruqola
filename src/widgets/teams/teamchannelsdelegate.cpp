/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelsdelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/teamroomsmodel.h"
#include <QPainter>
using namespace Qt::Literals::StringLiterals;
TeamChannelsDelegate::TeamChannelsDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
{
}

TeamChannelsDelegate::~TeamChannelsDelegate() = default;

void TeamChannelsDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

void TeamChannelsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? name (username)
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
    const Utils::AvatarInfo info = index.data(TeamRoomsModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, option.rect.height());
        if (!pix.isNull()) {
            drawDecoration(painter, option, displayRect, pix);
        }
        // Add extra size even if we don't have avatar pix
        xPos = margin + option.rect.height();
    }

    const QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(TeamRoomsModel::Name).toString();
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    painter->drawText(xPos + margin, defaultCharHeight, name);
    painter->setFont(oldFont);
}

QSize TeamChannelsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4 * option.widget->devicePixelRatioF());
}

#include "moc_teamchannelsdelegate.cpp"

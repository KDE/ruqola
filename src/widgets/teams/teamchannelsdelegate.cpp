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

    if (!option.showDecorationSelected && (option.state & QStyle::State_Selected)) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();
    const QPen oldPen = painter->pen();
    // The text is painted with QPainter::drawText(), so unlike drawDisplay() it doesn't switch
    // to QPalette::HighlightedText by itself.
    DelegatePaintUtil::setTextPen(painter, option);

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    const QFontMetrics fontMetrics(boldFont);

    int xPos = 0;
    const Utils::AvatarInfo info = index.data(TeamRoomsModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const int iconSize = option.rect.height() - 4;
        const int iconY = option.rect.y() + 2;
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            painter->drawPixmap(margin, iconY, iconSize, iconSize, pix);
        }
        // Add extra size even if we don't have avatar pix
        xPos = margin + iconSize;
    }

    const QString name = index.data(TeamRoomsModel::Name).toString();
    const int defaultCharHeight = option.rect.y() + (option.rect.height() - fontMetrics.height()) / 2 + fontMetrics.ascent();
    painter->drawText(xPos + margin, defaultCharHeight, name);
    painter->setFont(oldFont);
    painter->setPen(oldPen);
}

QSize TeamChannelsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4);
}

#include "moc_teamchannelsdelegate.cpp"

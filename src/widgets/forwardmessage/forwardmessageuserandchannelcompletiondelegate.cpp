/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "forwardmessageuserandchannelcompletiondelegate.h"
#include "common/delegatepaintutil.h"
#include "forwardmessagechannelmodel.h"
#include "misc/avatarcachemanager.h"

#include <KLocalizedString>

#include <QFontMetricsF>
#include <QPainter>

ForwardMessageUserAndChannelCompletionDelegate::ForwardMessageUserAndChannelCompletionDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::UserAndRoom, this))
{
}

ForwardMessageUserAndChannelCompletionDelegate::~ForwardMessageUserAndChannelCompletionDelegate() = default;

void ForwardMessageUserAndChannelCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? name
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
    const Utils::AvatarInfo info = index.data(ForwardMessageChannelModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        const QPixmap pix = mAvatarCacheManager->makeAvatarPixmap(option.widget, info, option.rect.height());
        if (!pix.isNull()) {
            drawDecoration(painter, option, displayRect, pix);
            xPos = margin + option.rect.height();
        }
    }

    QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(ForwardMessageChannelModel::Name).toString();
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    painter->drawText(xPos + margin, defaultCharHeight, name);
    painter->setFont(oldFont);
}

void ForwardMessageUserAndChannelCompletionDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "joinedchannelcompletiondelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/joinedchannelmodel.h"

#include <KLocalizedString>

#include <QPainter>

namespace
{
constexpr ushort padding = 4;
}

JoinedChannelCompletionDelegate::JoinedChannelCompletionDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::UserAndRoom, this))
{
}

JoinedChannelCompletionDelegate::~JoinedChannelCompletionDelegate() = default;

void JoinedChannelCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? name
    drawBackground(painter, option, index);

    if (!option.showDecorationSelected && (option.state & QStyle::State_Selected)) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    int xPos = -1;
    const Utils::AvatarInfo info = index.data(JoinedChannelModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const int iconSize = option.rect.height() - padding;
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            constexpr int marginTop = padding / 2;
            // Not drawDecoration(): it aligns the pixmap using its device size, which offsets it
            // when the devicePixelRatio is > 1.
            painter->drawPixmap(option.rect.x() + margin, option.rect.y() + marginTop, iconSize, iconSize, pix);
            xPos = margin + iconSize;
        }
    }

    QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(JoinedChannelModel::Name).toString();
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    painter->drawText(xPos + margin, defaultCharHeight, name);
    painter->setFont(oldFont);
}

void JoinedChannelCompletionDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize JoinedChannelCompletionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index) + QSize(0, padding);
}

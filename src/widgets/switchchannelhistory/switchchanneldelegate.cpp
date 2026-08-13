/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "switchchanneldelegate.h"

#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/switchchannelhistorymodel.h"
#include "rocketchataccount.h"

#include <QPainter>

namespace
{
constexpr int padding = 4;
}

using namespace Qt::Literals::StringLiterals;
SwitchChannelDelegate::SwitchChannelDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
{
    mAvatarCacheManager->setMaxEntries(15);
}

SwitchChannelDelegate::~SwitchChannelDelegate() = default;

void SwitchChannelDelegate::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(currentRocketChatAccount);
    mAvatarCacheManager->clearCache();
}

void SwitchChannelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <icon> [M] <name>
    drawBackground(painter, option, index);
    const QString text = index.data(SwitchChannelHistoryModel::Name).toString();
    const int margin = DelegatePaintUtil::margin();
    // The row is padding pixels taller than the text, so inset the avatar by it instead of
    // letting it fill the row edge to edge.
    const int iconSize = option.rect.height() - padding;
    int xPos = 0;
    const Utils::AvatarInfo info = index.data(SwitchChannelHistoryModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            // Don't use drawDecoration() here: it aligns the pixmap using its device size, so an
            // avatar with a devicePixelRatio > 1 ends up offset by half of it. Drawing into an
            // explicit target rect is devicePixelRatio-correct.
            painter->drawPixmap(option.rect.x() + margin, option.rect.y() + padding / 2, iconSize, iconSize, pix);
            xPos = margin + iconSize;
        }
    }
    const int xText = option.rect.x() + margin + xPos;

    const QRect displayRect(xText, option.rect.y(), option.rect.width() - margin - xPos, option.rect.height());
    drawDisplay(painter, option, displayRect, text);
}

QSize SwitchChannelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QItemDelegate::sizeHint(option, index) + QSize(0, padding);
    const Utils::AvatarInfo info = index.data(SwitchChannelHistoryModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        // Same layout as paint(): [M] <square avatar of (row height - padding)> [M] <name>
        size.rwidth() += 2 * DelegatePaintUtil::margin() + size.height() - padding;
    }
    return size;
}

#include "moc_switchchanneldelegate.cpp"

/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

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
constexpr uint padding = 2;
}

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
    mRocketChatAccount = currentRocketChatAccount;
}

void SwitchChannelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <icon> [M] <name>
    drawBackground(painter, option, index);
    const QString text = index.data(SwitchChannelHistoryModel::Name).toString();
    const int margin = DelegatePaintUtil::margin();
    int xPos = 0;
    const Utils::AvatarInfo info = index.data(SwitchChannelHistoryModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        const QPixmap pix = mAvatarCacheManager->makeAvatarPixmap(option.widget, info, option.rect.height());
        if (!pix.isNull()) {
            drawDecoration(painter, option, displayRect, pix);
            xPos = margin + option.rect.height();
        }
    }
    const int xText = option.rect.x() + margin + xPos;

    const QRect displayRect(xText, option.rect.y(), option.rect.width() - xText, option.rect.height());
    drawDisplay(painter, option, displayRect, text);
}

QSize SwitchChannelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index) + QSize(0, 2 * padding);
}

#include "moc_switchchanneldelegate.cpp"

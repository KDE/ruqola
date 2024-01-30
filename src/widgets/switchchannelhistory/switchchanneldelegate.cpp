/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "switchchanneldelegate.h"
#include "colorsandmessageviewstyle.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"

#include <KColorScheme>
#include <QPainter>

namespace
{
constexpr uint padding = 2;
}

SwitchChannelDelegate::SwitchChannelDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::Room, this))
{
}

SwitchChannelDelegate::~SwitchChannelDelegate() = default;

void SwitchChannelDelegate::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(currentRocketChatAccount);
    mRocketChatAccount = currentRocketChatAccount;
}

void SwitchChannelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <icon> [M] <name>
    drawBackground(painter, option, index);
}

QSize SwitchChannelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index) + QSize(0, 2 * padding);
}

#include "moc_switchchanneldelegate.cpp"

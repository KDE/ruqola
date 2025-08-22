/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploredatabaseroomiddelegate.h"
#include "rocketchataccount.h"

ExploreDatabaseRoomIdDelegate::ExploreDatabaseRoomIdDelegate(RocketChatAccount *account, QObject *parent)
    : QStyledItemDelegate{parent}
    , mRocketChatAccount(account)
{
}

ExploreDatabaseRoomIdDelegate::~ExploreDatabaseRoomIdDelegate() = default;

QString ExploreDatabaseRoomIdDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (mRocketChatAccount) {
        if (auto r = mRocketChatAccount->room(value.toByteArray()); r) {
            return r->displayRoomName();
        }
    }
    return QStyledItemDelegate::displayText(value, locale);
}

#include "moc_exploredatabaseroomiddelegate.cpp"

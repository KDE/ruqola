/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountroomsettings.h"

AccountRoomSettings::AccountRoomSettings() = default;

AccountRoomSettings::~AccountRoomSettings() = default;

void AccountRoomSettings::remove(const QString &roomId)
{
    mPendingTypedTexts.remove(roomId);
}

void AccountRoomSettings::add(const QString &roomId, const PendingTypedInfo &info)
{
    mPendingTypedTexts[roomId] = info;
}

AccountRoomSettings::PendingTypedInfo AccountRoomSettings::value(const QString &roomId)
{
    return mPendingTypedTexts.value(roomId);
}

bool AccountRoomSettings::isEmpty() const
{
    return mPendingTypedTexts.isEmpty();
}

bool AccountRoomSettings::PendingTypedInfo::isValid() const
{
    return !text.isEmpty() || !messageIdBeingEdited.isEmpty() || (scrollbarPosition != -1) || !threadMessageId.isEmpty() || !quotePermalink.isEmpty()
        || !quoteText.isEmpty();
}

QDebug operator<<(QDebug d, const AccountRoomSettings::PendingTypedInfo &t)
{
    d << " text " << t.text;
    d << " messageIdBeingEdited " << t.messageIdBeingEdited;
    d << " scrollbarPosition " << t.scrollbarPosition;
    d << " threadMessageId " << t.threadMessageId;
    d << " quotePermalink " << t.quotePermalink;
    d << " quoteText " << t.quoteText;
    return d;
}

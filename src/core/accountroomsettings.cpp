/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "accountroomsettings.h"

AccountRoomSettings::AccountRoomSettings()
{
}

AccountRoomSettings::~AccountRoomSettings()
{
}

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

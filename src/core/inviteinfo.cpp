/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "inviteinfo.h"

InviteInfo::InviteInfo()
{
}

// QJsonDocument([{"_id":"D2F6of","_updatedAt":"2021-04-07T06:49:04.571Z","createdAt":"2021-04-07T06:49:04.571Z","days":1,"expires":"2021-04-08T06:49:04.571Z"
// ,"maxUses":25,"rid":"n2GWePY4zjG48g7qA","userId":"H7Q9djXQ4iShzD9T2","uses":0}])

void InviteInfo::parseInviteInfo(const QJsonObject &replyObject)
{
    mIdentifier = replyObject[QLatin1String("_id")].toString();
    mUserIdentifier = replyObject[QLatin1String("userId")].toString();
    mRoomId = replyObject[QLatin1String("rid")].toString();
    mUses = replyObject[QLatin1String("uses")].toInt();
    mMaxUses = replyObject[QLatin1String("maxUses")].toInt();
    // TODO
    //    QDateTime mExpireDateTime;
    // QDateTime mCreateDateTime;
}

const QString &InviteInfo::identifier() const
{
    return mIdentifier;
}

void InviteInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

int InviteInfo::maxUses() const
{
    return mMaxUses;
}

void InviteInfo::setMaxUses(int newMaxUses)
{
    mMaxUses = newMaxUses;
}

const QString &InviteInfo::userIdentifier() const
{
    return mUserIdentifier;
}

void InviteInfo::setUserIdentifier(const QString &newUserIdentifier)
{
    mUserIdentifier = newUserIdentifier;
}

const QString &InviteInfo::roomId() const
{
    return mRoomId;
}

void InviteInfo::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

int InviteInfo::uses() const
{
    return mUses;
}

void InviteInfo::setUses(int newUses)
{
    mUses = newUses;
}

const QDateTime &InviteInfo::expireDateTime() const
{
    return mExpireDateTime;
}

void InviteInfo::setExpireDateTime(const QDateTime &newExpireDateTime)
{
    mExpireDateTime = newExpireDateTime;
}

const QDateTime &InviteInfo::createDateTime() const
{
    return mCreateDateTime;
}

void InviteInfo::setCreateDateTime(const QDateTime &newCreateDateTime)
{
    mCreateDateTime = newCreateDateTime;
}

QDebug operator<<(QDebug d, const InviteInfo &t)
{
    d << "mIdentifier: " << t.identifier();
    d << "mUserIdentifier: " << t.userIdentifier();
    d << "mMaxUses: " << t.maxUses();
    d << "v: " << t.uses();
    d << "mRoomId: " << t.roomId();
    d << "mExpireDateTime: " << t.expireDateTime();
    d << "mCreateDateTime: " << t.createDateTime();
    return d;
}

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

void InviteInfo::parseInviteInfo(const QJsonObject &replyObject)
{
    // TODO
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
    // TODO
    d << "mIdentifier: " << t.identifier();
    d << "mUserIdentifier: " << t.userIdentifier();
    d << "mMaxUses: " << t.maxUses();
    d << "v: " << t.uses();
    d << "mRoomId: " << t.roomId();
    d << "mExpireDateTime: " << t.expireDateTime();
    d << "mCreateDateTime: " << t.createDateTime();
    return d;
}

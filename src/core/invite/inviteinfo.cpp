/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteinfo.h"
#include "utils.h"

InviteInfo::InviteInfo() = default;

void InviteInfo::parseInviteInfo(const QJsonObject &replyObject)
{
    mIdentifier = replyObject[QLatin1String("_id")].toString();
    mUserIdentifier = replyObject[QLatin1String("userId")].toString();
    mRoomId = replyObject[QLatin1String("rid")].toString();
    mUses = replyObject[QLatin1String("uses")].toInt();
    mMaxUses = replyObject[QLatin1String("maxUses")].toInt();
    if (replyObject.contains(QLatin1String("createdAt"))) {
        setCreateDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("createdAt"), replyObject)));
    }
    if (replyObject.contains(QLatin1String("expires"))) {
        setExpireDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("expires"), replyObject)));
    }
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

bool InviteInfo::operator==(const InviteInfo &other) const
{
    return mUserIdentifier == other.userIdentifier() && mIdentifier == other.identifier() && mRoomId == other.roomId()
        && mExpireDateTime == other.expireDateTime() && mCreateDateTime == other.createDateTime() && mUses == other.uses() && mMaxUses == other.maxUses();
}

QDebug operator<<(QDebug d, const InviteInfo &t)
{
    d << "mIdentifier: " << t.identifier();
    d << "mUserIdentifier: " << t.userIdentifier();
    d << "mMaxUses: " << t.maxUses();
    d << "uses: " << t.uses();
    d << "mRoomId: " << t.roomId();
    d << "mExpireDateTime: " << t.expireDateTime();
    d << "mCreateDateTime: " << t.createDateTime();
    return d;
}

/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteinfo.h"
#include "utils.h"

InviteInfo::InviteInfo() = default;

void InviteInfo::parseInviteInfo(const QJsonObject &replyObject)
{
    mIdentifier = replyObject[QLatin1StringView("_id")].toString().toLatin1();
    mUserIdentifier = replyObject[QLatin1StringView("userId")].toString().toLatin1();
    mRoomId = replyObject[QLatin1StringView("rid")].toString().toLatin1();
    mUses = replyObject[QLatin1StringView("uses")].toInt();
    mMaxUses = replyObject[QLatin1StringView("maxUses")].toInt();
    if (replyObject.contains(QLatin1StringView("createdAt"))) {
        setCreateDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("createdAt"), replyObject)));
    }
    if (replyObject.contains(QLatin1StringView("expires"))) {
        setExpireDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("expires"), replyObject)));
    }
}

const QByteArray &InviteInfo::identifier() const
{
    return mIdentifier;
}

void InviteInfo::setIdentifier(const QByteArray &newIdentifier)
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

const QByteArray &InviteInfo::userIdentifier() const
{
    return mUserIdentifier;
}

void InviteInfo::setUserIdentifier(const QByteArray &newUserIdentifier)
{
    mUserIdentifier = newUserIdentifier;
}

const QByteArray &InviteInfo::roomId() const
{
    return mRoomId;
}

void InviteInfo::setRoomId(const QByteArray &newRoomId)
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
    d.space() << "mIdentifier:" << t.identifier();
    d.space() << "mUserIdentifier:" << t.userIdentifier();
    d.space() << "mMaxUses:" << t.maxUses();
    d.space() << "uses:" << t.uses();
    d.space() << "mRoomId:" << t.roomId();
    d.space() << "mExpireDateTime:" << t.expireDateTime();
    d.space() << "mCreateDateTime:" << t.createDateTime();
    return d;
}

/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteinfo.h"
#include "utils.h"

#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(InviteInfo, Ruqola_InviteInfo)
using namespace Qt::Literals::StringLiterals;
InviteInfo::InviteInfo() = default;

void InviteInfo::parseInviteInfo(const QJsonObject &replyObject)
{
    // qDebug() << " replyObject " << replyObject;
    mIdentifier = replyObject["_id"_L1].toString().toLatin1();
    mUserIdentifier = replyObject["userId"_L1].toString().toLatin1();
    mRoomId = replyObject["rid"_L1].toString().toLatin1();
    mUses = replyObject["uses"_L1].toInt();
    mMaxUses = replyObject["maxUses"_L1].toInt();
    if (replyObject.contains("createdAt"_L1)) {
        setCreateDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(u"createdAt"_s, replyObject)));
    }
    if (replyObject.contains("expires"_L1)) {
        setExpireDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(u"expires"_s, replyObject)));
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

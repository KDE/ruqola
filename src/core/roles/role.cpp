/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "role.h"
#include "ruqola_debug.h"

#include <QJsonArray>

Role::Role() = default;

QString Role::userId() const
{
    return mUserId;
}

void Role::setUserId(const QString &userId)
{
    mUserId = userId;
}

bool Role::isModerator() const
{
    return mIsModerator;
}

void Role::setIsModerator(bool isModerator)
{
    mIsModerator = isModerator;
}

bool Role::isLeader() const
{
    return mIsLeader;
}

void Role::setIsLeader(bool isLeader)
{
    mIsLeader = isLeader;
}

bool Role::isOwner() const
{
    return mIsOwner;
}

void Role::setIsOwner(bool isOwner)
{
    mIsOwner = isOwner;
}

bool Role::operator==(const Role &other) const
{
    return (mIsOwner == other.isOwner()) && (mIsModerator == other.isModerator()) && (mIsLeader == other.isLeader()) && (mUserId == other.userId());
}

bool Role::isValid() const
{
    return !mUserId.isEmpty();
}

void Role::updateRole(const QString &str, bool b)
{
    if (str == QLatin1String("moderator")) {
        mIsModerator = b;
    } else if (str == QLatin1String("leader")) {
        mIsLeader = b;
    } else if (str == QLatin1String("owner")) {
        mIsOwner = b;
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown role" << str;
    }
}

bool Role::hasARole() const
{
    return mIsOwner || mIsModerator || mIsLeader;
}

void Role::parseRole(const QJsonObject &obj)
{
    mUserId = obj.value(QLatin1String("u")).toObject().value(QLatin1String("_id")).toString();
    const QJsonArray roleArray = obj[QStringLiteral("roles")].toArray();
    for (int i = 0; i < roleArray.count(); ++i) {
        const QString str = roleArray.at(i).toString();
        updateRole(str, true);
    }
}

QDebug operator<<(QDebug d, const Role &t)
{
    d << "UserId: " << t.userId();
    d << "IsLeader: " << t.isLeader();
    d << "IsOwner: " << t.isOwner();
    d << "IsModerator: " << t.isModerator();
    return d;
}

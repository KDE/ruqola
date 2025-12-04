/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "role.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(Role, Ruqola_Role)

#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Role::Role() = default;

QByteArray Role::userId() const
{
    return mUserId;
}

void Role::setUserId(const QByteArray &userId)
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
    return (mIsOwner == other.isOwner()) && (mIsModerator == other.isModerator()) && (mIsLeader == other.isLeader()) && (mUserId == other.userId())
        && (mUserName == other.userName());
}

bool Role::isValid() const
{
    return !mUserId.isEmpty();
}

void Role::updateRole(const QString &str, bool b)
{
    if (str == "moderator"_L1) {
        mIsModerator = b;
    } else if (str == "leader"_L1) {
        mIsLeader = b;
    } else if (str == "owner"_L1) {
        mIsOwner = b;
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown role" << str;
    }
}

bool Role::hasARole() const
{
    return mIsOwner || mIsModerator || mIsLeader;
}

QString Role::userName() const
{
    return mUserName;
}

void Role::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

void Role::parseRole(const QJsonObject &obj)
{
    const QJsonObject uObj = obj.value("u"_L1).toObject();
    mUserId = uObj.value("_id"_L1).toString().toLatin1();
    mUserName = uObj.value("username"_L1).toString();
    const QJsonArray roleArray = obj["roles"_L1].toArray();
    for (int i = 0; i < roleArray.count(); ++i) {
        const QString str = roleArray.at(i).toString();
        updateRole(str, true);
    }
}

QDebug operator<<(QDebug d, const Role &t)
{
    d.space() << "UserId:" << t.userId();
    d.space() << "IsLeader:" << t.isLeader();
    d.space() << "IsOwner:" << t.isOwner();
    d.space() << "IsModerator:" << t.isModerator();
    d.space() << "UserName:" << t.userName();
    return d;
}

/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "user.h"
#include "utils.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

User::User()
{
}

User::~User()
{
}

QString User::name() const
{
    return mName;
}

void User::setName(const QString &name)
{
    mName = name;
}

QString User::userId() const
{
    return mUserId;
}

void User::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString User::status() const
{
    return mStatus;
}

void User::setStatus(const QString &status)
{
    mStatus = status;
}

bool User::operator ==(const User &other) const
{
    return (mName == other.name())
           && (mUserId == other.userId())
           && (mStatus == other.status())
           && (mUserName == other.userName())
           && (mUtcOffset == other.utcOffset())
           && (mStatusText == other.statusText())
           && (mRoles == other.roles());
}

bool User::operator !=(const User &other) const
{
    return !operator ==(other);
}

QString User::userName() const
{
    return mUserName;
}

void User::setUserName(const QString &userName)
{
    mUserName = userName;
}

bool User::isValid() const
{
    return !mUserName.isEmpty() || !mName.isEmpty();
}

double User::utcOffset() const
{
    return mUtcOffset;
}

void User::setUtcOffset(double utcOffset)
{
    mUtcOffset = utcOffset;
}

QString User::statusText() const
{
    return mStatusText;
}

void User::setStatusText(const QString &statusText)
{
    mStatusText = statusText;
}

QDebug operator <<(QDebug d, const User &t)
{
    d << "Name " << t.name();
    d << "UserId " << t.userId();
    d << "Status " << t.status();
    d << "UserName " << t.userName();
    d << "UtcOffset " << t.utcOffset();
    d << "StatusText " << t.statusText();
    d << "roles : " << t.roles();
    return d;
}

//FIXME Add autotest for it!
void User::parseUserRestApi(const QJsonObject &object)
{
    setUserId(object.value(QLatin1String("_id")).toString());
    setName(object.value(QLatin1String("name")).toString());
    setStatus(object.value(QLatin1String("status")).toString());
    setUserName(object.value(QLatin1String("username")).toString());
    setStatusText(object.value(QLatin1String("statusText")).toString());
    setUtcOffset(object.value(QLatin1String("utcOffset")).toDouble());
    const QJsonArray rolesArray = object.value(QStringLiteral("roles")).toArray();
    QStringList roles;
    for (int i = 0, total = rolesArray.size(); i < total; ++i) {
        roles.append(rolesArray.at(i).toString());
    }
    setRoles(roles);
    //TODO createdAt, emails, lastLogin
    //qDebug() << " object "  << object;
}

void User::parseUser(const QVariantList &list)
{
    if (list.count() != 4) {
        qCWarning(RUQOLA_LOG) << " List argument different of 4! It's a bug";
        return;
    }
    setUserId(list.at(0).toString());
    setUserName(list.at(1).toString());
    const int valueStatus = list.at(2).toInt();
    if (valueStatus == 0) {
        setStatus(QStringLiteral("offline"));
    } else if (valueStatus == 1) {
        setStatus(QStringLiteral("online"));
    } else if (valueStatus == 2) {
        setStatus(QStringLiteral("away"));
    } else if (valueStatus == 3) {
        setStatus(QStringLiteral("busy"));
    } else {
        qCWarning(RUQOLA_LOG) << " Invalid status value" << valueStatus;
        return;
    }
    const QVariant customText = list.at(3);
    if (customText.isValid()) {
        setStatusText(customText.toString());
    }
}

QStringList User::roles() const
{
    return mRoles;
}

void User::setRoles(const QStringList &roles)
{
    mRoles = roles;
}

void User::parseUser(const QJsonObject &object)
{
    const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
    setUserId(object.value(QLatin1String("id")).toString());
    setName(fields.value(QLatin1String("name")).toString());
    setStatus(fields.value(QLatin1String("status")).toString());
    setUserName(fields.value(QLatin1String("username")).toString());
    setStatusText(fields.value(QLatin1String("statusText")).toString());
    setUtcOffset(fields.value(QLatin1String("utcOffset")).toDouble());
}

QString User::iconFromStatus() const
{
    return Utils::iconFromStatus(mStatus);
}

QVector<User> User::parseUsersList(const QJsonObject &object)
{
    const QJsonArray fieldsArray = object.value(QLatin1String("items")).toArray();
    QVector<User> users;
    for (const QJsonValue &current : fieldsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject userObject = current.toObject();
            User user;
            user.parseUserRestApi(userObject);
            users.append(user);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing users" << current;
        }
    }
    return users;
}

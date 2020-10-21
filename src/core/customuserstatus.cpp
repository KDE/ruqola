/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "customuserstatus.h"

#include "utils.h"

CustomUserStatus::CustomUserStatus()
{
}

CustomUserStatus::~CustomUserStatus()
{
}

bool CustomUserStatus::isValid() const
{
    return !mIdentifier.isEmpty() && !mStatusType.isEmpty();
}

void CustomUserStatus::parseCustomStatus(const QJsonObject &customStatusObj, bool useRestApi)
{
    mIdentifier = customStatusObj[QLatin1String("_id")].toString();
    mName = customStatusObj[QLatin1String("name")].toString();
    mStatusType = customStatusObj[QLatin1String("statusType")].toString();
    if (customStatusObj.contains(QLatin1String("_updatedAt"))) {
        if (useRestApi) {
            mUpdatedAt = Utils::parseIsoDate(QStringLiteral("_updatedAt"), customStatusObj);
        } else {
            mUpdatedAt = Utils::parseDate(QStringLiteral("_updatedAt"), customStatusObj);
        }
    }
}

QString CustomUserStatus::name() const
{
    return mName;
}

void CustomUserStatus::setName(const QString &value)
{
    mName = value;
}

qint64 CustomUserStatus::updatedAt() const
{
    return mUpdatedAt;
}

void CustomUserStatus::setUpdatedAt(const qint64 &updatedAt)
{
    mUpdatedAt = updatedAt;
}

QString CustomUserStatus::identifier() const
{
    return mIdentifier;
}

void CustomUserStatus::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QString CustomUserStatus::statusType() const
{
    return mStatusType;
}

void CustomUserStatus::setStatusType(const QString &statusType)
{
    mStatusType = statusType;
}

QDebug operator <<(QDebug d, const CustomUserStatus &t)
{
    d << "name " << t.name() << '\n';
    d << "identifier " << t.identifier() << '\n';
    d << "updatedAt " << t.updatedAt() << '\n';
    d << "StatusType " << t.statusType() << '\n';
    return d;
}

bool CustomUserStatus::operator ==(const CustomUserStatus &other) const
{
    return mIdentifier == other.identifier()
           && mName == other.name()
           && mStatusType == other.statusType()
           && mUpdatedAt == other.updatedAt();
}

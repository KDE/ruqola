/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatus.h"

#include "utils.h"

CustomUserStatus::CustomUserStatus() = default;

CustomUserStatus::~CustomUserStatus() = default;

bool CustomUserStatus::isValid() const
{
    return !mIdentifier.isEmpty() && (mStatusType != User::PresenceStatus::Unknown);
}

void CustomUserStatus::parseCustomStatus(const QJsonObject &customStatusObj, bool useRestApi)
{
    mIdentifier = customStatusObj[QLatin1String("_id")].toString();
    mName = customStatusObj[QLatin1String("name")].toString();
    mStatusType = Utils::presenceStatusFromString(customStatusObj[QLatin1String("statusType")].toString());
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

void CustomUserStatus::setUpdatedAt(qint64 updatedAt)
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

User::PresenceStatus CustomUserStatus::statusType() const
{
    return mStatusType;
}

void CustomUserStatus::setStatusType(User::PresenceStatus statusType)
{
    mStatusType = statusType;
}

QDebug operator<<(QDebug d, const CustomUserStatus &t)
{
    d.space() << "name" << t.name() << '\n';
    d.space() << "identifier" << t.identifier() << '\n';
    d.space() << "updatedAt" << t.updatedAt() << '\n';
    d.space() << "StatusType" << t.statusType() << '\n';
    return d;
}

bool CustomUserStatus::operator==(const CustomUserStatus &other) const
{
    return mIdentifier == other.identifier() && mName == other.name() && mStatusType == other.statusType() && mUpdatedAt == other.updatedAt();
}

/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatus.h"
#include "utils.h"

#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(CustomUserStatus, Ruqola_CustomUserStatus)
using namespace Qt::Literals::StringLiterals;
CustomUserStatus::CustomUserStatus() = default;

CustomUserStatus::~CustomUserStatus() = default;

bool CustomUserStatus::isValid() const
{
    return !mIdentifier.isEmpty() && (mStatusType != User::PresenceStatus::Unknown);
}

void CustomUserStatus::parseCustomStatus(const QJsonObject &customStatusObj, bool useRestApi)
{
    mIdentifier = customStatusObj["_id"_L1].toString().toLatin1();
    mName = customStatusObj["name"_L1].toString();
    mStatusType = Utils::presenceStatusFromString(customStatusObj["statusType"_L1].toString());
    if (customStatusObj.contains("_updatedAt"_L1)) {
        if (useRestApi) {
            mUpdatedAt = Utils::parseIsoDate(u"_updatedAt"_s, customStatusObj);
        } else {
            mUpdatedAt = Utils::parseDate(u"_updatedAt"_s, customStatusObj);
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

QByteArray CustomUserStatus::identifier() const
{
    return mIdentifier;
}

void CustomUserStatus::setIdentifier(const QByteArray &identifier)
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

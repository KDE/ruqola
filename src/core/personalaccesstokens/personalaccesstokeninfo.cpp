/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfo.h"
#include "utils.h"

#include <QLocale>

PersonalAccessTokenInfo::PersonalAccessTokenInfo() = default;

QDebug operator<<(QDebug d, const PersonalAccessTokenInfo &t)
{
    d << "mBypassTwoFactor " << t.bypassTwoFactor();
    d << "mName " << t.name();
    d << "mLastTokenPart " << t.lastTokenPart();
    d << "mCreatedAt " << t.createdAt();
    return d;
}

void PersonalAccessTokenInfo::parsePersonalAccessTokenInfo(const QJsonObject &replyObject)
{
    mName = replyObject.value(QLatin1String("name")).toString();
    mLastTokenPart = replyObject.value(QLatin1String("lastTokenPart")).toString();
    mBypassTwoFactor = replyObject.value(QLatin1String("bypassTwoFactor")).toBool();
    setCreatedAt(Utils::parseIsoDate(QStringLiteral("createdAt"), replyObject));
}

qint64 PersonalAccessTokenInfo::createdAt() const
{
    return mCreatedAt;
}

void PersonalAccessTokenInfo::setCreatedAt(qint64 newCreatedAt)
{
    mCreatedAt = newCreatedAt;
    if (mCreatedAt != -1) {
        QLocale l;
        mCreateAtDisplayDateTime = l.toString(QDateTime::fromMSecsSinceEpoch(mCreatedAt), QLocale::LongFormat);
    }
}

const QString &PersonalAccessTokenInfo::createAtDisplayDateTime() const
{
    return mCreateAtDisplayDateTime;
}

bool PersonalAccessTokenInfo::bypassTwoFactor() const
{
    return mBypassTwoFactor;
}

void PersonalAccessTokenInfo::setBypassTwoFactor(bool newBypassTwoFactor)
{
    mBypassTwoFactor = newBypassTwoFactor;
}

const QString &PersonalAccessTokenInfo::name() const
{
    return mName;
}

void PersonalAccessTokenInfo::setName(const QString &newName)
{
    mName = newName;
}

const QString &PersonalAccessTokenInfo::lastTokenPart() const
{
    return mLastTokenPart;
}

void PersonalAccessTokenInfo::setLastTokenPart(const QString &newLastTokenPart)
{
    mLastTokenPart = newLastTokenPart;
}

bool PersonalAccessTokenInfo::isValid() const
{
    return !mName.isEmpty() && !mLastTokenPart.isEmpty() && (mCreatedAt != -1);
}

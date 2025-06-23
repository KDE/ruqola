/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfo.h"

#include "utils.h"

#include <QLocale>

using namespace Qt::Literals::StringLiterals;
PersonalAccessTokenInfo::PersonalAccessTokenInfo() = default;

QDebug operator<<(QDebug d, const PersonalAccessTokenInfo &t)
{
    d.space() << "mBypassTwoFactor" << t.bypassTwoFactor();
    d.space() << "mName" << t.name();
    d.space() << "mLastTokenPart" << t.lastTokenPart();
    d.space() << "mCreatedAt" << t.createdAt();
    return d;
}

void PersonalAccessTokenInfo::parsePersonalAccessTokenInfo(const QJsonObject &replyObject)
{
    mName = replyObject.value("name"_L1).toString();
    mLastTokenPart = replyObject.value("lastTokenPart"_L1).toString();
    mBypassTwoFactor = replyObject.value("bypassTwoFactor"_L1).toBool();
    setCreatedAt(Utils::parseIsoDate(u"createdAt"_s, replyObject));
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
    return !mLastTokenPart.isEmpty() && (mCreatedAt != -1);
}

bool PersonalAccessTokenInfo::operator==(const PersonalAccessTokenInfo &other) const
{
    return other.lastTokenPart() == lastTokenPart() && other.createdAt() == createdAt() && other.name() == name()
        && other.bypassTwoFactor() == bypassTwoFactor();
}

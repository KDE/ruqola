/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthinfo.h"

#include "utils.h"

OauthInfo::OauthInfo() = default;

QDebug operator<<(QDebug d, const OauthInfo &t)
{
    d << "Identifier " << t.identifier();
    d << "Active " << t.active();
    d << "Name " << t.name();
    d << "ClientId " << t.clientId();
    d << "ClientSecret " << t.clientSecret();
    d << "RedirectUri " << t.redirectUri();
    d << "CreatedBy" << t.createdBy();
    return d;
}

bool OauthInfo::operator==(const OauthInfo &other) const
{
    return mIdentifier == other.identifier() && mActive == other.active() && mName == other.name() && mClientId == other.clientId()
        && mClientSecret == other.clientSecret() && mRedirectUri == other.redirectUri() && mCreatedBy == other.createdBy();
}

void OauthInfo::parseOauthInfo(const QJsonObject &replyObject, bool restApi)
{
    mIdentifier = replyObject[QLatin1String("_id")].toString();
    mActive = replyObject[QLatin1String("active")].toBool();
    mName = replyObject[QLatin1String("name")].toString();
    mClientId = replyObject[QLatin1String("clientId")].toString();
    mClientSecret = replyObject[QLatin1String("clientSecret")].toString();
    mRedirectUri = replyObject[QLatin1String("redirectUri")].toString();
    // TODO _updatedAt
    const QJsonObject createdBy = replyObject[QLatin1String("_createdBy")].toObject();
    mCreatedBy = createdBy[QLatin1String("username")].toString();
    // {"_id":"system","username":"system"}
    if (replyObject.contains(QLatin1String("_createdAt"))) {
        if (restApi) {
            setCreatedDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(QStringLiteral("_createdAt"), replyObject)));
        } else {
            setCreatedDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseDate(QStringLiteral("_createdAt"), replyObject)));
        }
    }
}

const QString &OauthInfo::identifier() const
{
    return mIdentifier;
}

void OauthInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

bool OauthInfo::active() const
{
    return mActive;
}

void OauthInfo::setActive(bool newActive)
{
    mActive = newActive;
}

const QString &OauthInfo::name() const
{
    return mName;
}

void OauthInfo::setName(const QString &newName)
{
    mName = newName;
}

const QString &OauthInfo::clientId() const
{
    return mClientId;
}

void OauthInfo::setClientId(const QString &newClientId)
{
    mClientId = newClientId;
}

const QString &OauthInfo::clientSecret() const
{
    return mClientSecret;
}

void OauthInfo::setClientSecret(const QString &newClientSecret)
{
    mClientSecret = newClientSecret;
}

const QString &OauthInfo::redirectUri() const
{
    return mRedirectUri;
}

void OauthInfo::setRedirectUri(const QString &newRedirectUri)
{
    mRedirectUri = newRedirectUri;
}

const QString &OauthInfo::createdBy() const
{
    return mCreatedBy;
}

void OauthInfo::setCreatedBy(const QString &newCreatedBy)
{
    mCreatedBy = newCreatedBy;
}

const QDateTime &OauthInfo::createdDateTime() const
{
    return mCreatedDateTime;
}

void OauthInfo::setCreatedDateTime(const QDateTime &newCreatedDateTime)
{
    mCreatedDateTime = newCreatedDateTime;
}

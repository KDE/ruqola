/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthinfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(OauthInfo, Ruqola_OauthInfo)

#include "utils.h"

using namespace Qt::Literals::StringLiterals;
OauthInfo::OauthInfo() = default;

QDebug operator<<(QDebug d, const OauthInfo &t)
{
    d.space() << "Identifier" << t.identifier();
    d.space() << "Active" << t.active();
    d.space() << "Name" << t.name();
    d.space() << "ClientId" << t.clientId();
    d.space() << "ClientSecret" << t.clientSecret();
    d.space() << "RedirectUri" << t.redirectUri();
    d.space() << "CreatedBy" << t.createdBy();
    return d;
}

bool OauthInfo::operator==(const OauthInfo &other) const
{
    return mIdentifier == other.identifier() && mActive == other.active() && mName == other.name() && mClientId == other.clientId()
        && mClientSecret == other.clientSecret() && mRedirectUri == other.redirectUri() && mCreatedBy == other.createdBy();
}

void OauthInfo::parseOauthInfo(const QJsonObject &replyObject, bool restApi)
{
    mIdentifier = replyObject["_id"_L1].toString();
    mActive = replyObject["active"_L1].toBool();
    mName = replyObject["name"_L1].toString();
    mClientId = replyObject["clientId"_L1].toString();
    mClientSecret = replyObject["clientSecret"_L1].toString();
    mRedirectUri = replyObject["redirectUri"_L1].toString();
    // TODO _updatedAt
    const QJsonObject createdBy = replyObject["_createdBy"_L1].toObject();
    mCreatedBy = createdBy["username"_L1].toString();
    // {"_id":"system","username":"system"}
    if (replyObject.contains("_createdAt"_L1)) {
        if (restApi) {
            setCreatedDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseIsoDate(u"_createdAt"_s, replyObject)));
        } else {
            setCreatedDateTime(QDateTime::fromMSecsSinceEpoch(Utils::parseDate(u"_createdAt"_s, replyObject)));
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

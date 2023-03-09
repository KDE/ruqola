/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ownuser/ownuser.h"
#include "utils.h"
#include <QJsonArray>

OwnUser::OwnUser() = default;

OwnUser::~OwnUser() = default;

void OwnUser::parseOwnUserInfo(const QJsonObject &replyObject)
{
    // qDebug() << "replyObject "<<replyObject;
    mUserId = replyObject.value(QLatin1String("_id")).toString();
    mUserName = replyObject.value(QLatin1String("username")).toString();
    mStatus = replyObject.value(QLatin1String("status")).toString();
    mEmail = replyObject.value(QLatin1String("email")).toString();
    mStatusText = replyObject.value(QLatin1String("statusText")).toString();
    mName = replyObject.value(QLatin1String("name")).toString();
    mAvatarUrl = replyObject.value(QLatin1String("avatarUrl")).toString();
    mUtcOffset = replyObject.value(QLatin1String("utcOffset")).toDouble();
    mStatusDefault = replyObject.value(QLatin1String("statusDefault")).toString();
    mNickName = replyObject.value(QLatin1String("nickname")).toString();
    const QJsonArray array = replyObject.value(QLatin1String("roles")).toArray();
    const QVariantList rolesLst = array.toVariantList();
    mRoles.clear();
    mRoles.reserve(rolesLst.count());
    for (const auto &role : rolesLst) {
        mRoles.append(role.toString());
    }
    mServicePassword.parseService(replyObject.value(QLatin1String("services")).toObject());
    mOwnUserPreferences.parsePreferences(replyObject.value(QLatin1String("settings")).toObject().value(QLatin1String("preferences")).toObject());
}

bool OwnUser::isAdministrator() const
{
    return mRoles.contains(QStringLiteral("admin"));
}

ServicePassword OwnUser::servicePassword() const
{
    return mServicePassword;
}

void OwnUser::setServicePassword(const ServicePassword &servicePassword)
{
    mServicePassword = servicePassword;
}

OwnUserPreferences OwnUser::ownUserPreferences() const
{
    return mOwnUserPreferences;
}

void OwnUser::setOwnUserPreferences(const OwnUserPreferences &ownUserPreferences)
{
    mOwnUserPreferences = ownUserPreferences;
}

QString OwnUser::userId() const
{
    return mUserId;
}

void OwnUser::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString OwnUser::userName() const
{
    return mUserName;
}

void OwnUser::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString OwnUser::status() const
{
    return mStatus;
}

void OwnUser::setStatus(const QString &status)
{
    mStatus = status;
}

QDebug operator<<(QDebug d, const OwnUser &t)
{
    d << "UserId " << t.userId();
    d << "Status " << t.status();
    d << "UserName " << t.userName();
    d << "Email " << t.email();
    d << "StatusText " << t.statusText();
    d << "Name " << t.name();
    d << "AvatarUrl " << t.avatarUrl();
    d << "utcOffset " << t.utcOffset();
    d << "defaultStatus " << t.statusDefault();
    d << "nickname " << t.nickName();
    d << "roles " << t.roles();
    d << "servicePassword " << t.servicePassword();
    d << "OwnUserPreferences " << t.ownUserPreferences();
    return d;
}

bool OwnUser::operator==(const OwnUser &other) const
{
    return (mUserId == other.userId()) && (mStatus == other.status()) && (mUserName == other.userName()) && (mEmail == other.email())
        && (mStatusText == other.statusText()) && (mName == other.name()) && (mAvatarUrl == other.avatarUrl()) && (mUtcOffset == other.utcOffset())
        && (mStatusDefault == other.statusDefault()) && (mNickName == other.nickName()) && (mRoles == other.roles())
        && (mServicePassword == other.servicePassword()) && (mOwnUserPreferences == other.ownUserPreferences());
}

QString OwnUser::email() const
{
    return mEmail;
}

void OwnUser::setEmail(const QString &email)
{
    mEmail = email;
}

QString OwnUser::statusText() const
{
    return mStatusText;
}

void OwnUser::setStatusText(const QString &statusText)
{
    mStatusText = statusText;
}

QString OwnUser::name() const
{
    return mName;
}

void OwnUser::setName(const QString &name)
{
    mName = name;
}

QString OwnUser::avatarUrl() const
{
    return mAvatarUrl;
}

void OwnUser::setAvatarUrl(const QString &avatarUrl)
{
    mAvatarUrl = avatarUrl;
}

double OwnUser::utcOffset() const
{
    return mUtcOffset;
}

void OwnUser::setUtcOffset(double utcOffset)
{
    mUtcOffset = utcOffset;
}

QString OwnUser::statusDefault() const
{
    return mStatusDefault;
}

void OwnUser::setStatusDefault(const QString &statusDefault)
{
    mStatusDefault = statusDefault;
}

User OwnUser::user() const
{
    User user;
    user.setUserId(mUserId);
    user.setUserName(mUserName);
    user.setStatus(Utils::presenceStatusFromString(mStatus));
    return user;
}

QString OwnUser::nickName() const
{
    return mNickName;
}

void OwnUser::setNickName(const QString &nickName)
{
    mNickName = nickName;
}

QStringList OwnUser::roles() const
{
    return mRoles;
}

void OwnUser::setRoles(const QStringList &roles)
{
    mRoles = roles;
}

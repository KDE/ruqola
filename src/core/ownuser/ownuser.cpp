/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ownuser/ownuser.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(OwnUser, Ruqola_OwnUser)

#include "utils.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
OwnUser::OwnUser() = default;

OwnUser::~OwnUser() = default;

void OwnUser::parseOwnUserInfo(const QJsonObject &replyObject)
{
    // qDebug() << "replyObject " << replyObject;
    mUserId = replyObject.value("_id"_L1).toString().toLatin1();
    mUserName = replyObject.value("username"_L1).toString();
    mStatus = replyObject.value("status"_L1).toString();
    mEmail = replyObject.value("email"_L1).toString();
    mStatusText = replyObject.value("statusText"_L1).toString();
    mName = replyObject.value("name"_L1).toString();
    mAvatarUrl = replyObject.value("avatarUrl"_L1).toString();
    mUtcOffset = replyObject.value("utcOffset"_L1).toDouble();
    mStatusDefault = replyObject.value("statusDefault"_L1).toString();
    mNickName = replyObject.value("nickname"_L1).toString();
    const QJsonArray array = replyObject.value("roles"_L1).toArray();
    const QVariantList rolesLst = array.toVariantList();
    mRoles.clear();
    mRoles.reserve(rolesLst.count());
    for (const auto &role : rolesLst) {
        mRoles.append(role.toString());
    }
    mServicePassword.parseService(replyObject.value("services"_L1).toObject());
    mOwnUserPreferences.parsePreferences(replyObject.value("settings"_L1).toObject().value("preferences"_L1).toObject());
}

bool OwnUser::isAdministrator() const
{
    return mRoles.contains(u"admin"_s);
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

QByteArray OwnUser::userId() const
{
    return mUserId;
}

void OwnUser::setUserId(const QByteArray &userId)
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
    d.space() << "UserId" << t.userId();
    d.space() << "Status" << t.status();
    d.space() << "UserName" << t.userName();
    d.space() << "Email" << t.email();
    d.space() << "StatusText" << t.statusText();
    d.space() << "Name" << t.name();
    d.space() << "AvatarUrl" << t.avatarUrl();
    d.space() << "utcOffset" << t.utcOffset();
    d.space() << "defaultStatus" << t.statusDefault();
    d.space() << "nickname" << t.nickName();
    d.space() << "roles" << t.roles();
    d.space() << "servicePassword" << t.servicePassword();
    d.space() << "OwnUserPreferences" << t.ownUserPreferences();
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

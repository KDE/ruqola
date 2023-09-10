/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userbasejob.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;

UserBaseJob::UserBaseJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UserBaseJob::~UserBaseJob() = default;

UserBaseJob::UserInfo UserBaseJob::userInfo() const
{
    return mUserInfo;
}

void UserBaseJob::setUserInfo(const UserInfo &userInfo)
{
    mUserInfo = userInfo;
}

bool UserBaseJob::hasUserIdentifier() const
{
    return !mUserInfo.userIdentifier.isEmpty() && (mUserInfo.userInfoType != UserBaseJob::UserInfoType::Unknown);
}

void UserBaseJob::addQueryUrl(QUrl &url) const
{
    QUrlQuery queryUrl;
    if (mUserInfo.userInfoType == UserBaseJob::UserInfoType::UserName) {
        queryUrl.addQueryItem(QStringLiteral("username"), mUserInfo.userIdentifier);
    } else {
        queryUrl.addQueryItem(QStringLiteral("userId"), mUserInfo.userIdentifier);
    }
    url.setQuery(queryUrl);
}

QString UserBaseJob::identifier() const
{
    switch (mUserInfo.userInfoType) {
    case UserBaseJob::UserInfoType::UserName:
    case UserBaseJob::UserInfoType::UserId:
        return mUserInfo.userIdentifier;
    case UserBaseJob::UserInfoType::Unknown:
        return {};
    }
    return {};
}

void UserBaseJob::generateJson(QJsonObject &jsonObj) const
{
    switch (mUserInfo.userInfoType) {
    case UserBaseJob::UserInfoType::UserName:
        jsonObj[QLatin1String("username")] = mUserInfo.userIdentifier;
        break;
    case UserBaseJob::UserInfoType::UserId:
        jsonObj[QLatin1String("userId")] = mUserInfo.userIdentifier;
        break;
    case UserBaseJob::UserInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UserBaseJob::UserInfoType::Unknown is a bug !";
        break;
    }
}

#include "moc_userbasejob.cpp"

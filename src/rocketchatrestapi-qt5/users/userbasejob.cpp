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

#include "userbasejob.h"

#include <QUrlQuery>
using namespace RocketChatRestApi;

UserBaseJob::UserBaseJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UserBaseJob::~UserBaseJob()
{
}

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

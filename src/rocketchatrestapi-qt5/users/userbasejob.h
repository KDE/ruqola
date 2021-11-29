/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UserBaseJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class UserInfoType {
        Unknown,
        UserId,
        UserName,
    };

    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UserInfo {
        QString userIdentifier;
        UserBaseJob::UserInfoType userInfoType = UserBaseJob::UserInfoType::Unknown;
    };
    explicit UserBaseJob(QObject *parent = nullptr);
    ~UserBaseJob() override;

    Q_REQUIRED_RESULT UserInfo userInfo() const;
    void setUserInfo(const UserInfo &userInfo);

    Q_REQUIRED_RESULT bool hasUserIdentifier() const;

    Q_REQUIRED_RESULT QString identifier() const;

protected:
    void addQueryUrl(QUrl &url) const;
    void generateJson(QJsonObject &jsonObj) const;
    UserInfo mUserInfo;
};
}


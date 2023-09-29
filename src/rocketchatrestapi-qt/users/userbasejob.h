/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UserBaseJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class UserInfoType {
        Unknown,
        UserId,
        UserName,
    };

    struct LIBROCKETCHATRESTAPI_QT_EXPORT UserInfo {
        QString userIdentifier;
        UserBaseJob::UserInfoType userInfoType = UserBaseJob::UserInfoType::Unknown;
    };
    explicit UserBaseJob(QObject *parent = nullptr);
    ~UserBaseJob() override;

    [[nodiscard]] UserInfo userInfo() const;
    void setUserInfo(const UserInfo &userInfo);

    [[nodiscard]] bool hasUserIdentifier() const;

    [[nodiscard]] QString identifier() const;

protected:
    void addQueryUrl(QUrl &url) const;
    void generateJson(QJsonObject &jsonObj) const;
    UserInfo mUserInfo;
};
}

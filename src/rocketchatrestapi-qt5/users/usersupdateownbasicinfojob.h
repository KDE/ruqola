/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QDebug>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UsersUpdateOwnBasicInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UpdateOwnBasicInfo {
        enum class BasicInfoType { Unknown = 0, Email = 1, Name = 2, UserName = 4, NickName = 8, StatusText = 16, Password = 32 };
        Q_DECLARE_FLAGS(BasicInfoTypes, BasicInfoType)

        BasicInfoTypes type = {};
        QString email;
        QString name;
        QString userName;
        QString nickName;
        QString statusText;
        QByteArray currentPassword;
        QString newPassword;
        Q_REQUIRED_RESULT bool isValid() const;
        // Add custom field ?
    };

    explicit UsersUpdateOwnBasicInfoJob(QObject *parent = nullptr);
    ~UsersUpdateOwnBasicInfoJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT UpdateOwnBasicInfo updateOwnBasicInfo() const;
    void setUpdateOwnBasicInfo(const UpdateOwnBasicInfo &updateOwnBasicInfo);

Q_SIGNALS:
    void updateOwnBasicInfoDone();
    void passwordChanged();

private:
    Q_DISABLE_COPY(UsersUpdateOwnBasicInfoJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UpdateOwnBasicInfo mUpdateOwnBasicInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo &t);

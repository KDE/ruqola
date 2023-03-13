/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createupdateuserinfo.h"
#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UsersUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UsersUpdateJob(QObject *parent = nullptr);
    ~UsersUpdateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT UpdateUserInfo updateInfo() const;
    void setUpdateInfo(const UpdateUserInfo &updateInfo);
    Q_REQUIRED_RESULT bool requireTwoFactorAuthentication() const override;

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void usersUpdateDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersUpdateJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UpdateUserInfo mUpdateInfo;
};
}

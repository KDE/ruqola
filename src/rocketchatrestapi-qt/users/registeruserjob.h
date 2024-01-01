/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RegisterUserJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RegisterUserInfo {
        QString username;
        QString name;
        QString email;
        QString password;
        [[nodiscard]] bool isValid() const;
    };

    explicit RegisterUserJob(QObject *parent = nullptr);
    ~RegisterUserJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] RegisterUserInfo registerUserInfo() const;
    void setRegisterUserInfo(const RegisterUserInfo &registerUserInfo);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void registerUserDone();

private:
    Q_DISABLE_COPY(RegisterUserJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RegisterUserInfo mRegisterUserInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::RegisterUserJob::RegisterUserInfo, Q_MOVABLE_TYPE);

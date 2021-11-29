/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RegisterUserJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RegisterUserInfo {
        QString username;
        QString name;
        QString email;
        QString password;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    explicit RegisterUserJob(QObject *parent = nullptr);
    ~RegisterUserJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT RegisterUserInfo registerUserInfo() const;
    void setRegisterUserInfo(const RegisterUserInfo &registerUserInfo);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void registerUserDone();

private:
    Q_DISABLE_COPY(RegisterUserJob)
    void slotRegisterUser();
    RegisterUserInfo mRegisterUserInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::RegisterUserJob::RegisterUserInfo, Q_MOVABLE_TYPE);

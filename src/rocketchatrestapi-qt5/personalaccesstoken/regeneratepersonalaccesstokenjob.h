/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RegeneratePersonalAccessTokenJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RegeneratePersonalAccessTokenJob(QObject *parent = nullptr);
    ~RegeneratePersonalAccessTokenJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool requireTwoFactorAuthentication() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT const QString &tokenName() const;
    void setTokenName(const QString &newTokenName);

Q_SIGNALS:
    void regenerateTokenDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(RegeneratePersonalAccessTokenJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mTokenName;
};
}

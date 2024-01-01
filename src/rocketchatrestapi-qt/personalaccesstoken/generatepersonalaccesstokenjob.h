/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GeneratePersonalAccessTokenJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GeneratePersonalAccessTokenJob(QObject *parent = nullptr);
    ~GeneratePersonalAccessTokenJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] const QString &tokenName() const;
    void setTokenName(const QString &newTokenName);

    [[nodiscard]] bool bypassTwoFactor() const;
    void setBypassTwoFactor(bool newBypassTwoFactor);

Q_SIGNALS:
    void generateTokenDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GeneratePersonalAccessTokenJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTokenName;
    bool mBypassTwoFactor = false;
};
}

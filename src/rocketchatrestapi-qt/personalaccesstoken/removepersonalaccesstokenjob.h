/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RemovePersonalAccessTokenJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RemovePersonalAccessTokenJob(QObject *parent = nullptr);
    ~RemovePersonalAccessTokenJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] const QString &tokenName() const;
    void setTokenName(const QString &newTokenName);

Q_SIGNALS:
    void removeTokenDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(RemovePersonalAccessTokenJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTokenName;
};
}

/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT User2FADisableEmailJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit User2FADisableEmailJob(QObject *parent = nullptr);
    ~User2FADisableEmailJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

protected:
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

Q_SIGNALS:
    void disableEmailDone();

private:
    Q_DISABLE_COPY(User2FADisableEmailJob)
};
}

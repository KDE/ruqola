/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ForgotPasswordJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // since 0.64
    explicit ForgotPasswordJob(QObject *parent = nullptr);
    ~ForgotPasswordJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString email() const;
    void setEmail(const QString &email);

    [[nodiscard]] QJsonDocument json() const;

Q_SIGNALS:
    void forgotPasswordDone();

private:
    Q_DISABLE_COPY(ForgotPasswordJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mEmail;
};
}

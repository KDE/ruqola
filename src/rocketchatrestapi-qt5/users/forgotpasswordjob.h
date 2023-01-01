/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ForgotPasswordJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // since 0.64
    explicit ForgotPasswordJob(QObject *parent = nullptr);
    ~ForgotPasswordJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString email() const;
    void setEmail(const QString &email);

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void forgotPasswordDone();

private:
    Q_DISABLE_COPY(ForgotPasswordJob)
    void onPostRequestResponse(const QJsonDocument &reply) override;
    QString mEmail;
};
}

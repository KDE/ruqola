/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT User2FASendEmailCodeJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit User2FASendEmailCodeJob(QObject *parent = nullptr);
    ~User2FASendEmailCodeJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString usernameOrEmail() const;
    void setUsernameOrEmail(const QString &usernameOrEmail);

protected:
    void onPostRequestResponse(const QJsonDocument &replyJson) override;

Q_SIGNALS:
    void sendEmailCodeDone();

private:
    Q_DISABLE_COPY(User2FASendEmailCodeJob)
    void slotSendEmailCode();
    QString mUsernameOrEmail;
};
}

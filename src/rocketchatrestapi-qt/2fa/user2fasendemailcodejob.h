/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT User2FASendEmailCodeJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit User2FASendEmailCodeJob(QObject *parent = nullptr);
    ~User2FASendEmailCodeJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString usernameOrEmail() const;
    void setUsernameOrEmail(const QString &usernameOrEmail);

protected:
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

Q_SIGNALS:
    void sendEmailCodeDone();

private:
    Q_DISABLE_COPY(User2FASendEmailCodeJob)
    void slotSendEmailCode();
    QString mUsernameOrEmail;
};
}

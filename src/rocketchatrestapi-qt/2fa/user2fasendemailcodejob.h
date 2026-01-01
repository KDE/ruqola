/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT User2FASendEmailCodeJob : public RestApiAbstractJob
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
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

Q_SIGNALS:
    void sendEmailCodeDone();

private:
    QString mUsernameOrEmail;
};
}

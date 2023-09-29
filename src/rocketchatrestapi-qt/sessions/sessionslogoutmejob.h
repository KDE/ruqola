/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SessionsLogoutMeJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SessionsLogoutMeJob(QObject *parent = nullptr);
    ~SessionsLogoutMeJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] const QString &sessionId() const;
    void setSessionId(const QString &newSessionId);

Q_SIGNALS:
    void logoutMeDone();

private:
    Q_DISABLE_COPY(SessionsLogoutMeJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mSessionId;
};
}

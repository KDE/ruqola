/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT SessionsLogoutMeJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SessionsLogoutMeJob(QObject *parent = nullptr);
    ~SessionsLogoutMeJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT const QString &sessionId() const;
    void setSessionId(const QString &newSessionId);

Q_SIGNALS:
    void logoutMeDone();

private:
    Q_DISABLE_COPY(SessionsLogoutMeJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mSessionId;
};
}

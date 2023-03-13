/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT CustomUserStatusDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomUserStatusDeleteJob(QObject *parent = nullptr);
    ~CustomUserStatusDeleteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString customUserStatusId() const;
    void setCustomUserStatusId(const QString &customUserStatusId);

Q_SIGNALS:
    void userStatusDeletedDone();

private:
    Q_DISABLE_COPY(CustomUserStatusDeleteJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mCustomUserStatusId;
};
}

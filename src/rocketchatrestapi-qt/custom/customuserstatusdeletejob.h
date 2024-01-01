/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT CustomUserStatusDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomUserStatusDeleteJob(QObject *parent = nullptr);
    ~CustomUserStatusDeleteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString customUserStatusId() const;
    void setCustomUserStatusId(const QString &customUserStatusId);

Q_SIGNALS:
    void userStatusDeletedDone();

private:
    Q_DISABLE_COPY(CustomUserStatusDeleteJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mCustomUserStatusId;
};
}

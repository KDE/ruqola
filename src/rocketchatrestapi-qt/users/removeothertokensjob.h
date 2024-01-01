/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT RemoveOtherTokensJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RemoveOtherTokensJob(QObject *parent = nullptr);
    ~RemoveOtherTokensJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
Q_SIGNALS:
    void removeOtherTokensDone();

private:
    Q_DISABLE_COPY(RemoveOtherTokensJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}

/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ResetOwnE2eKeyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ResetOwnE2eKeyJob(QObject *parent = nullptr);
    ~ResetOwnE2eKeyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

Q_SIGNALS:
    void resetE2eKeyDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(ResetOwnE2eKeyJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}

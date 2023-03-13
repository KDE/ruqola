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

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void resetE2eKeyDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(ResetOwnE2eKeyJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}

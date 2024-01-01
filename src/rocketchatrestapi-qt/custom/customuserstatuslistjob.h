/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT CustomUserStatusListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomUserStatusListJob(QObject *parent = nullptr);
    ~CustomUserStatusListJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool hasQueryParameterSupport() const override;
Q_SIGNALS:
    void customUserStatusDone(const QJsonObject &data);

private:
    Q_DISABLE_COPY(CustomUserStatusListJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}

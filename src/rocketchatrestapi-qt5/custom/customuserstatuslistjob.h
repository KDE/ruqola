/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT CustomUserStatusListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CustomUserStatusListJob(QObject *parent = nullptr);
    ~CustomUserStatusListJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;
Q_SIGNALS:
    void customUserStatusDone(const QJsonObject &data);

private:
    Q_DISABLE_COPY(CustomUserStatusListJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT GetBannersJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetBannersJob(QObject *parent = nullptr);
    ~GetBannersJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void getBannersDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetBannersJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

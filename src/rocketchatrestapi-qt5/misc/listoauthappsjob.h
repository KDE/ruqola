/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ListOauthAppsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ListOauthAppsJob(QObject *parent = nullptr);
    ~ListOauthAppsJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

Q_SIGNALS:
    void settingsOauthDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(ListOauthAppsJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

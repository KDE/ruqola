/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SettingsOauthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SettingsOauthJob(QObject *parent = nullptr);
    ~SettingsOauthJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

Q_SIGNALS:
    void settingsOauthDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(SettingsOauthJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

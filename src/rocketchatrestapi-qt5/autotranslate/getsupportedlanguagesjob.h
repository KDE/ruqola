/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetSupportedLanguagesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetSupportedLanguagesJob(QObject *parent = nullptr);
    ~GetSupportedLanguagesJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void getSupportedLanguagesDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetSupportedLanguagesJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

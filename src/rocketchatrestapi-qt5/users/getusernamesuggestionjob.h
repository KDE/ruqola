/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetUsernameSuggestionJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // Since 0.65
    explicit GetUsernameSuggestionJob(QObject *parent = nullptr);
    ~GetUsernameSuggestionJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void getUsernameSuggestionDone(const QString &result);

private:
    Q_DISABLE_COPY(GetUsernameSuggestionJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

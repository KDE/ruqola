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
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT FetchMyKeysJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit FetchMyKeysJob(QObject *parent = nullptr);
    ~FetchMyKeysJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void fetchMyKeysDone();

private:
    Q_DISABLE_COPY(FetchMyKeysJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

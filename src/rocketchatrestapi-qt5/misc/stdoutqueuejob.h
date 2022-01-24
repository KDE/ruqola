/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT StdoutQueueJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit StdoutQueueJob(QObject *parent = nullptr);
    ~StdoutQueueJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

Q_SIGNALS:
    void stdoutQueueDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(StdoutQueueJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

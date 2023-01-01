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
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ListCommandsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // since 0.60.2
    explicit ListCommandsJob(QObject *parent = nullptr);
    ~ListCommandsJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

Q_SIGNALS:
    void listCommandsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(ListCommandsJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
};
}

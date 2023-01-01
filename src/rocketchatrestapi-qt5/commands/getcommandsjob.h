/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetCommandsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // since 0.60.2
    explicit GetCommandsJob(QObject *parent = nullptr);
    ~GetCommandsJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString commandName() const;
    void setCommandName(const QString &commandName);

Q_SIGNALS:
    void getCommandsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetCommandsJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    QString mCommandName;
};
}

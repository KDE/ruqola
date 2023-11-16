/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT GetCommandsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // since 0.60.2
    explicit GetCommandsJob(QObject *parent = nullptr);
    ~GetCommandsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString commandName() const;
    void setCommandName(const QString &commandName);

Q_SIGNALS:
    void getCommandsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetCommandsJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mCommandName;
};
}

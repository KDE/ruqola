/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChannelListJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelListJob(QObject *parent = nullptr);
    ~ChannelListJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void channelListDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(ChannelListJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &reply) override;
};
}

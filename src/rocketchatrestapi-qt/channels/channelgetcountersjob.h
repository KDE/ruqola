/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelGetCountersJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelGetCountersJob(QObject *parent = nullptr);
    ~ChannelGetCountersJob() override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void channelGetCountersDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelGetCountersJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}

/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChannelInfoJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelInfoJob(QObject *parent = nullptr);
    ~ChannelInfoJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void channelInfoDone(const QJsonObject &replyObjectj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelInfoJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}

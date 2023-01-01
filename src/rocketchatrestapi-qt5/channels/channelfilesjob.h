/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelFilesJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    enum ChannelType {
        Channel = 0,
        Groups,
        Direct,
        Unknown,
    };
    Q_ENUM(ChannelType)
    explicit ChannelFilesJob(QObject *parent = nullptr);
    ~ChannelFilesJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT ChannelType channelType() const;
    void setChannelType(RocketChatRestApi::ChannelFilesJob::ChannelType channelType);

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;
Q_SIGNALS:
    void channelFilesDone(const QJsonObject &replyObjectj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelFilesJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    ChannelType mChannelType = Unknown;
};
}

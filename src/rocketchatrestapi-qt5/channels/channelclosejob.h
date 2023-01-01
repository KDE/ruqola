/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelCloseJob : public ChannelGroupBaseJob
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
    explicit ChannelCloseJob(QObject *parent = nullptr);
    ~ChannelCloseJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT ChannelType channelType() const;
    void setChannelType(RocketChatRestApi::ChannelCloseJob::ChannelType channelType);

Q_SIGNALS:
    void closeChannelDone();

private:
    Q_DISABLE_COPY(ChannelCloseJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    ChannelType mChannelType = Unknown;
};
}

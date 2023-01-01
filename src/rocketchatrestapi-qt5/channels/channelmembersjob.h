/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelMembersJob : public ChannelGroupBaseJob
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
    explicit ChannelMembersJob(QObject *parent = nullptr);
    ~ChannelMembersJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT ChannelType channelType() const;
    void setChannelType(RocketChatRestApi::ChannelMembersJob::ChannelType channelType);

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void channelMembersDone(const QJsonObject &replyObjectj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

protected:
    Q_REQUIRED_RESULT QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(ChannelMembersJob)
    void onGetRequestResponse(const QJsonDocument &replyJson) override;
    ChannelType mChannelType = Unknown;
};
}

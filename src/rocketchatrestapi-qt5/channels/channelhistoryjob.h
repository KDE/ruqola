/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt5_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ChannelHistoryJob : public ChannelGroupBaseJob
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

    struct LIBROCKETCHATRESTAPI_QT5_EXPORT ChannelHistoryInfo {
        QString roomId;
        QString latestMessage;
        QString oldestMessage;
        ChannelType channelType = Unknown;
        int offset = -1; // default
        int count = -1;
        bool inclusive = false; // Default
        bool unreads = false;
    };

    explicit ChannelHistoryJob(QObject *parent = nullptr);
    ~ChannelHistoryJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT ChannelHistoryInfo channelHistoryInfo() const;
    void setChannelHistoryInfo(const ChannelHistoryInfo &channelHistoryInfo);

Q_SIGNALS:
    void channelHistoryDone(const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelHistoryJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    ChannelHistoryInfo mChannelHistoryInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &t);

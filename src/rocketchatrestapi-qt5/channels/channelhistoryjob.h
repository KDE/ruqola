/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    void slotLoadHistoryChannelFinished();
    ChannelHistoryInfo mChannelHistoryInfo;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo)
Q_DECLARE_TYPEINFO(RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &t);

/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef CHANNELFILESJOB_H
#define CHANNELFILESJOB_H

#include "restapiabstractjob.h"
#include "librestapi_private_export.h"
#include "channelbasejob.h"
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelFilesJob : public ChannelBaseJob
{
    Q_OBJECT
public:
    enum ChannelType {
        Channel = 0,
        Groups,
        Direct,
        Unknown
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
    void channelFilesDone(const QJsonObject &replyObjectj, const ChannelBaseJob::ChannelInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelFilesJob)
    void slotFilesinChannelFinished();
    ChannelType mChannelType = Unknown;
};
}

#endif // CHANNELFILESJOB_H

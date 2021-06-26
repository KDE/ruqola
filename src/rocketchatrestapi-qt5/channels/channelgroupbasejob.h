/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QJsonObject>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelGroupBaseJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelGroupBaseJob(QObject *parent = nullptr);
    ~ChannelGroupBaseJob() override;
    enum class ChannelInfoType {
        Unknown,
        RoomId,
        RoomName,
    };
    Q_ENUM(ChannelInfoType)

    struct ChannelInfo {
        QString channelInfoIdentifier;
        ChannelGroupBaseJob::ChannelInfoType channelInfoType = ChannelGroupBaseJob::ChannelInfoType::Unknown;
    };

    Q_REQUIRED_RESULT bool hasRoomIdentifier() const;

    Q_REQUIRED_RESULT ChannelInfo channelInfo() const;
    void setChannelInfo(const ChannelInfo &channelInfo);

protected:
    void generateJson(QJsonObject &obj) const;
    void addQueryItem(QUrl &url) const;

private:
    ChannelInfo mChannelInfo;
};
}
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelGroupBaseJob::ChannelInfo &t);

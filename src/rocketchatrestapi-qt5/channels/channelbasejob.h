/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef CHANNELBASEJOB_H
#define CHANNELBASEJOB_H

#include "restapiabstractjob.h"
#include "librestapi_private_export.h"

#include <QJsonObject>
#include <QUrlQuery>
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelBaseJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelBaseJob(QObject *parent = nullptr);
    ~ChannelBaseJob() override;
    enum class ChannelInfoType {
        Unknown,
        RoomId,
        RoomName,
    };

    struct ChannelInfo {
        QString channelInfoIdentifier;
        ChannelBaseJob::ChannelInfoType channelInfoType = ChannelBaseJob::ChannelInfoType::Unknown;
    };

    Q_REQUIRED_RESULT bool hasRoomIdentifier() const;

    Q_REQUIRED_RESULT ChannelInfo channelInfo() const;
    void setChannelInfo(const ChannelInfo &channelInfo);

protected:
    void generateJSon(QJsonObject &obj) const;
    void generateQuery(QUrlQuery &queryUrl) const;

private:
    ChannelInfo mChannelInfo;
};
}

#endif // CHANNELBASEJOB_H

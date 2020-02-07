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

#include "channelbasejob.h"
#include "rocketchatqtrestapi_debug.h"
using namespace RocketChatRestApi;
ChannelBaseJob::ChannelBaseJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelBaseJob::~ChannelBaseJob()
{
}

QString ChannelBaseJob::roomId() const
{
    return mRoomId;
}

void ChannelBaseJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChannelBaseJob::roomName() const
{
    return mRoomName;
}

void ChannelBaseJob::setRoomName(const QString &roomName)
{
    mRoomName = roomName;
}

bool ChannelBaseJob::hasRoomIdentifier() const
{
    //return !mChannelInfo.channelInfoIdentifier.isEmpty() && (mChannelInfo.channelInfoType != ChannelBaseJob::ChannelInfoType::Unknown);
    return !mRoomId.isEmpty() || !mRoomName.isEmpty();
}

void ChannelBaseJob::generateJSon(QJsonObject &jsonObj) const
{
#if 0
    switch(mChannelInfo.channelInfoType) {
    case ChannelBaseJob::ChannelInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelBaseJob::ChannelInfoType::RoomId:
        jsonObj[QLatin1String("roomId")] = roomId();
        break;
    case ChannelBaseJob::ChannelInfoType::RoomName:
        jsonObj[QLatin1String("roomName")] = roomName();
        break;
    }
#endif
    if (!mRoomId.isEmpty()) {
        jsonObj[QLatin1String("roomId")] = roomId();
    } else if (!mRoomName.isEmpty()) {
        jsonObj[QLatin1String("roomName")] = roomName();
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "roomId and roomName are empty. It's a bug!";
    }
}

void RocketChatRestApi::ChannelBaseJob::generateQuery(QUrlQuery &queryUrl) const
{
#if 0
    switch(mChannelInfo.channelInfoType) {
    case ChannelBaseJob::ChannelInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelBaseJob::ChannelInfoType::RoomId:
        queryUrl.addQueryItem(QStringLiteral("roomId"), roomId());
        break;
    case ChannelBaseJob::ChannelInfoType::RoomName:
        queryUrl.addQueryItem(QStringLiteral("roomName"), roomName());
        break;
    }
#endif
    if (!mRoomId.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("roomId"), roomId());
    } else if (!mRoomName.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("roomName"), roomName());
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "roomId and roomName are empty. It's a bug!";
    }
}

ChannelBaseJob::ChannelInfo ChannelBaseJob::channelInfo() const
{
    return mChannelInfo;
}

void ChannelBaseJob::setChannelInfo(const ChannelInfo &channelInfo)
{
    mChannelInfo = channelInfo;
}

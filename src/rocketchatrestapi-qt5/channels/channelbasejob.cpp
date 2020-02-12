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

bool ChannelBaseJob::hasRoomIdentifier() const
{
    return !mChannelInfo.channelInfoIdentifier.isEmpty() && (mChannelInfo.channelInfoType != ChannelBaseJob::ChannelInfoType::Unknown);
}

void ChannelBaseJob::generateJSon(QJsonObject &jsonObj) const
{
    switch (mChannelInfo.channelInfoType) {
    case ChannelBaseJob::ChannelInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelBaseJob::ChannelInfoType::RoomId:
        jsonObj[QLatin1String("roomId")] = mChannelInfo.channelInfoIdentifier;
        break;
    case ChannelBaseJob::ChannelInfoType::RoomName:
        jsonObj[QLatin1String("roomName")] = mChannelInfo.channelInfoIdentifier;
        break;
    }
}

void RocketChatRestApi::ChannelBaseJob::generateQuery(QUrlQuery &queryUrl) const
{
    switch (mChannelInfo.channelInfoType) {
    case ChannelBaseJob::ChannelInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelBaseJob::ChannelInfoType::RoomId:
        queryUrl.addQueryItem(QStringLiteral("roomId"), mChannelInfo.channelInfoIdentifier);
        break;
    case ChannelBaseJob::ChannelInfoType::RoomName:
        queryUrl.addQueryItem(QStringLiteral("roomName"), mChannelInfo.channelInfoIdentifier);
        break;
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

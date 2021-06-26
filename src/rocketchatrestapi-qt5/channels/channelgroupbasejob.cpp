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

#include "channelgroupbasejob.h"
#include "rocketchatqtrestapi_debug.h"
#include <QUrlQuery>
using namespace RocketChatRestApi;
ChannelGroupBaseJob::ChannelGroupBaseJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelGroupBaseJob::~ChannelGroupBaseJob()
{
}

bool ChannelGroupBaseJob::hasRoomIdentifier() const
{
    return !mChannelInfo.channelInfoIdentifier.isEmpty() && (mChannelInfo.channelInfoType != ChannelGroupBaseJob::ChannelInfoType::Unknown);
}

void ChannelGroupBaseJob::addQueryItem(QUrl &url) const
{
    QUrlQuery queryUrl;
    switch (mChannelInfo.channelInfoType) {
    case ChannelGroupBaseJob::ChannelInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelGroupBaseJob::ChannelInfoType::RoomId:
        queryUrl.addQueryItem(QStringLiteral("roomId"), mChannelInfo.channelInfoIdentifier);
        break;
    case ChannelGroupBaseJob::ChannelInfoType::RoomName:
        queryUrl.addQueryItem(QStringLiteral("roomName"), mChannelInfo.channelInfoIdentifier);
        break;
    }
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
}

void ChannelGroupBaseJob::generateJson(QJsonObject &jsonObj) const
{
    switch (mChannelInfo.channelInfoType) {
    case ChannelGroupBaseJob::ChannelInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelGroupBaseJob::ChannelInfoType::RoomId:
        jsonObj[QLatin1String("roomId")] = mChannelInfo.channelInfoIdentifier;
        break;
    case ChannelGroupBaseJob::ChannelInfoType::RoomName:
        jsonObj[QLatin1String("roomName")] = mChannelInfo.channelInfoIdentifier;
        break;
    }
}

ChannelGroupBaseJob::ChannelInfo ChannelGroupBaseJob::channelInfo() const
{
    return mChannelInfo;
}

void ChannelGroupBaseJob::setChannelInfo(const ChannelInfo &channelInfo)
{
    mChannelInfo = channelInfo;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelGroupBaseJob::ChannelInfo &t)
{
    d << "channelInfoIdentifier " << t.channelInfoIdentifier;
    d << "channelInfoType " << t.channelInfoType;
    return d;
}

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

ChannelGroupBaseJob::~ChannelGroupBaseJob() = default;

bool ChannelGroupBaseJob::hasIdentifier() const
{
    return !mChannelGroupInfo.identifier.isEmpty() && (mChannelGroupInfo.channelGroupInfoType != ChannelGroupBaseJob::ChannelGroupInfoType::Unknown);
}

void ChannelGroupBaseJob::addQueryItem(QUrl &url) const
{
    QUrlQuery queryUrl;
    switch (mChannelGroupInfo.channelGroupInfoType) {
    case ChannelGroupBaseJob::ChannelGroupInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelGroupBaseJob::ChannelGroupInfoType::Identifier:
        queryUrl.addQueryItem(QStringLiteral("roomId"), mChannelGroupInfo.identifier);
        break;
    case ChannelGroupBaseJob::ChannelGroupInfoType::Name:
        queryUrl.addQueryItem(QStringLiteral("roomName"), mChannelGroupInfo.identifier);
        break;
    }
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
}

void ChannelGroupBaseJob::generateJson(QJsonObject &jsonObj) const
{
    switch (mChannelGroupInfo.channelGroupInfoType) {
    case ChannelGroupBaseJob::ChannelGroupInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Type is unknown. It's a bug!";
        return;
    case ChannelGroupBaseJob::ChannelGroupInfoType::Identifier:
        jsonObj[QLatin1String("roomId")] = mChannelGroupInfo.identifier;
        break;
    case ChannelGroupBaseJob::ChannelGroupInfoType::Name:
        jsonObj[QLatin1String("roomName")] = mChannelGroupInfo.identifier;
        break;
    }
}

ChannelGroupBaseJob::ChannelGroupInfo ChannelGroupBaseJob::channelGroupInfo() const
{
    return mChannelGroupInfo;
}

void ChannelGroupBaseJob::setChannelGroupInfo(const ChannelGroupInfo &channelInfo)
{
    mChannelGroupInfo = channelInfo;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &t)
{
    d << "channelInfoIdentifier " << t.identifier;
    d << "channelInfoType " << t.channelGroupInfoType;
    return d;
}

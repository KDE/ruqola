/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

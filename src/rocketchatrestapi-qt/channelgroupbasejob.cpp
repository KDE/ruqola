/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelgroupbasejob.h"

#include "rocketchatqtrestapi_debug.h"
#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
        queryUrl.addQueryItem(u"roomId"_s, mChannelGroupInfo.identifier);
        break;
    case ChannelGroupBaseJob::ChannelGroupInfoType::Name:
        queryUrl.addQueryItem(u"roomName"_s, mChannelGroupInfo.identifier);
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
        jsonObj["roomId"_L1] = mChannelGroupInfo.identifier;
        break;
    case ChannelGroupBaseJob::ChannelGroupInfoType::Name:
        jsonObj["roomName"_L1] = mChannelGroupInfo.identifier;
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
    d.space() << "channelInfoIdentifier " << t.identifier;
    d.space() << "channelInfoType " << t.channelGroupInfoType;
    return d;
}

#include "moc_channelgroupbasejob.cpp"

/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createchannelteaminfo.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>


using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;

bool CreateChannelTeamInfo::isValid() const
{
    return !name.isEmpty();
}

bool CreateChannelTeamInfo::canStart() const
{
    return isValid();
}

QJsonDocument CreateChannelTeamInfo::json() const
{
    QJsonObject jsonObj;
    if (!members.isEmpty()) {
        jsonObj["members"_L1] = QJsonArray::fromStringList(members);
    }
    jsonObj["name"_L1] = name;
    if (readOnly) {
        jsonObj["readOnly"_L1] = true;
    } // Default is false

    if (infoType == CreateInfoType::Team) {
        jsonObj["type"_L1] = privateChannel ? 1 : 0;
    }

    QJsonObject extraJsonObj;

    if (broadcast) {
        extraJsonObj["broadcast"_L1] = true;
    } // Default is false
    if (encrypted) {
        extraJsonObj["encrypted"_L1] = true;
    } // Default is false
    if (!topic.isEmpty()) {
        extraJsonObj["topic"_L1] = topic;
    }
    if (federated) {
        extraJsonObj["federated"_L1] = true;
    }
    if (!teamId.isEmpty()) {
        extraJsonObj["teamId"_L1] = QLatin1StringView(teamId);
    }
    jsonObj["extraData"_L1] = extraJsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::CreateChannelTeamInfo &t)
{
    d.space() << "name " << t.name;
    d.space() << "description " << t.topic;
    d.space() << "teamId " << t.teamId;
    d.space() << "members " << t.members;
    d.space() << "readOnly " << t.readOnly;
    d.space() << "broadcast " << t.broadcast;
    d.space() << "encrypted " << t.encrypted;
    d.space() << "private channel " << t.privateChannel;
    d.space() << "infoType " << t.infoType;
    return d;
}

#include "moc_createchannelteaminfo.cpp"

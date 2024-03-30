/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createchannelteaminfo.h"

#include <QJsonArray>
#include <QJsonObject>

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
        jsonObj[QLatin1StringView("members")] = QJsonArray::fromStringList(members);
    }
    jsonObj[QLatin1StringView("name")] = name;
    if (readOnly) {
        jsonObj[QLatin1StringView("readOnly")] = true;
    } // Default is false

    if (infoType == Team) {
        jsonObj[QLatin1StringView("type")] = privateChannel ? 1 : 0;
    }

    QJsonObject extraJsonObj;

    if (broadcast) {
        extraJsonObj[QLatin1StringView("broadcast")] = true;
    } // Default is false
    if (encrypted) {
        extraJsonObj[QLatin1StringView("encrypted")] = true;
    } // Default is false
    if (!description.isEmpty()) {
        extraJsonObj[QLatin1StringView("description")] = description;
    }
    if (!teamId.isEmpty()) {
        extraJsonObj[QLatin1StringView("teamId")] = QString::fromLatin1(teamId);
    }
    jsonObj[QLatin1StringView("extraData")] = extraJsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::CreateChannelTeamInfo &t)
{
    d << "name " << t.name;
    d << "description " << t.description;
    d << "teamId " << t.teamId;
    d << "members " << t.members;
    d << "readOnly " << t.readOnly;
    d << "broadcast " << t.broadcast;
    d << "encrypted " << t.encrypted;
    d << "private channel " << t.privateChannel;
    d << "infoType " << t.infoType;
    return d;
}

#include "moc_createchannelteaminfo.cpp"

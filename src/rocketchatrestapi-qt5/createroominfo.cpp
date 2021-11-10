/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
#include "createroominfo.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace RocketChatRestApi;

bool RocketChatRestApi::CreateRoomInfo::isValid() const
{
    return !name.isEmpty();
}

bool CreateRoomInfo::canStart() const
{
    return !name.isEmpty();
}

QJsonDocument CreateRoomInfo::json() const
{
    QJsonObject jsonObj;
    if (!members.isEmpty()) {
        jsonObj[QLatin1String("members")] = QJsonArray::fromStringList(members);
    }
    jsonObj[QLatin1String("name")] = name;
    if (readOnly) {
        jsonObj[QLatin1String("readOnly")] = true;
    } // Default is false

    QJsonObject extraJsonObj;

    if (broadcast) {
        extraJsonObj[QLatin1String("broadcast")] = true;
    } // Default is false
    if (encrypted) {
        extraJsonObj[QLatin1String("encrypted")] = true;
    } // Default is false
    if (!description.isEmpty()) {
        extraJsonObj[QLatin1String("description")] = description;
    }
    if (!teamId.isEmpty()) {
        extraJsonObj[QLatin1String("teamId")] = teamId;
    }
    jsonObj[QLatin1String("extraData")] = extraJsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::CreateRoomInfo &t)
{
    d << "name " << t.name;
    d << "description " << t.description;
    d << "teamId " << t.teamId;
    d << "members " << t.members;
    d << "readOnly " << t.readOnly;
    d << "broadcast " << t.broadcast;
    d << "encrypted " << t.encrypted;
    return d;
}

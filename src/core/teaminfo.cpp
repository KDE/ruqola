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

#include "teaminfo.h"

TeamInfo::TeamInfo()
{
}

void TeamInfo::parseTeamInfo(const QJsonObject &replyObject)
{
    mTeamId = replyObject.value(QStringLiteral("teamId")).toString();
    mMainTeam = replyObject.value(QStringLiteral("teamMain")).toBool(false);
}

QJsonObject TeamInfo::serialize(const TeamInfo &retention)
{
    QJsonObject obj;
    obj[QStringLiteral("teamId")] = retention.teamId();
    obj[QStringLiteral("teamMain")] = retention.mainTeam();
    return obj;
}

TeamInfo TeamInfo::fromJSon(const QJsonObject &obj)
{
    TeamInfo info;
    info.setMainTeam(obj[QStringLiteral("teamMain")].toBool());
    info.setTeamId(obj[QStringLiteral("teamId")].toString());
    return info;
}

QString TeamInfo::teamId() const
{
    return mTeamId;
}

void TeamInfo::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

bool TeamInfo::mainTeam() const
{
    return mMainTeam;
}

void TeamInfo::setMainTeam(bool mainTeam)
{
    mMainTeam = mainTeam;
}

QDebug operator<<(QDebug d, const TeamInfo &t)
{
    d << "team id: " << t.teamId();
    d << "is Main Team: " << t.mainTeam();
    return d;
}

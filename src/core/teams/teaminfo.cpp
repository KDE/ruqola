/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teaminfo.h"

TeamInfo::TeamInfo() = default;

void TeamInfo::parseTeamInfo(const QJsonObject &replyObject)
{
    mTeamId = replyObject.value(QStringLiteral("teamId")).toString();
    mMainTeam = replyObject.value(QStringLiteral("teamMain")).toBool(false);
    mAutoJoin = replyObject.value(QStringLiteral("teamDefault")).toBool(false);
    mRoomsCount = replyObject.value(QStringLiteral("roomsCount")).toInt(0);
    //    if (isValid()) {
    //        // TODO add specific debug category ?
    //        qDebug() << " END team info " << *this;
    //    }
}

void TeamInfo::serialize(const TeamInfo &teams, QJsonObject &obj)
{
    if (teams.isValid()) {
        obj[QLatin1String("teamId")] = teams.teamId();
        obj[QLatin1String("teamMain")] = teams.mainTeam();
        if (teams.autoJoin()) {
            obj[QLatin1String("teamDefault")] = true;
        }
        const auto roomsCount = teams.roomsCount();
        if (roomsCount > 0) {
            obj[QLatin1String("roomsCount")] = roomsCount;
        }
    }
}

TeamInfo TeamInfo::deserialize(const QJsonObject &obj)
{
    TeamInfo info;
    info.setMainTeam(obj[QLatin1String("teamMain")].toBool());
    info.setTeamId(obj[QLatin1String("teamId")].toString());
    info.setAutoJoin(obj[QLatin1String("teamDefault")].toBool());
    info.setRoomsCount(obj[QLatin1String("roomsCount")].toInt(0));
    return info;
}

bool TeamInfo::isValid() const
{
    return !mTeamId.isEmpty();
}

bool TeamInfo::operator==(const TeamInfo &other) const
{
    return mTeamId == other.teamId() && mMainTeam == other.mainTeam() && mAutoJoin == other.autoJoin() && mRoomsCount == other.roomsCount();
}

bool TeamInfo::operator!=(const TeamInfo &other) const
{
    return !operator==(other);
}

bool TeamInfo::autoJoin() const
{
    return mAutoJoin;
}

void TeamInfo::setAutoJoin(bool autoJoin)
{
    mAutoJoin = autoJoin;
}

int TeamInfo::roomsCount() const
{
    return mRoomsCount;
}

void TeamInfo::setRoomsCount(int newRoomsCount)
{
    mRoomsCount = newRoomsCount;
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

bool TeamInfo::hasTeamRoom() const
{
    return !mMainTeam && !mTeamId.isEmpty();
}

QDebug operator<<(QDebug d, const TeamInfo &t)
{
    d << "team id: " << t.teamId();
    d << "is Main Team: " << t.mainTeam();
    d << "autojoin: " << t.autoJoin();
    d << "roomCount: " << t.roomsCount();
    return d;
}

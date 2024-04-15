/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teaminfo.h"
using namespace Qt::Literals::StringLiterals;

TeamInfo::TeamInfo() = default;

void TeamInfo::parseTeamInfo(const QJsonObject &replyObject)
{
    mTeamId = replyObject.value(QStringLiteral("teamId")).toString().toLatin1();
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
        obj["teamId"_L1] = QLatin1StringView(teams.teamId());
        obj["teamMain"_L1] = teams.mainTeam();
        if (teams.autoJoin()) {
            obj["teamDefault"_L1] = true;
        }
        const auto roomsCount = teams.roomsCount();
        if (roomsCount > 0) {
            obj["roomsCount"_L1] = roomsCount;
        }
    }
}

TeamInfo TeamInfo::deserialize(const QJsonObject &obj)
{
    TeamInfo info;
    info.setMainTeam(obj["teamMain"_L1].toBool());
    info.setTeamId(obj["teamId"_L1].toString().toLatin1());
    info.setAutoJoin(obj["teamDefault"_L1].toBool());
    info.setRoomsCount(obj["roomsCount"_L1].toInt(0));
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

QByteArray TeamInfo::teamId() const
{
    return mTeamId;
}

void TeamInfo::setTeamId(const QByteArray &teamId)
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

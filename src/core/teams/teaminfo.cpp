/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teaminfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(TeamInfo, Ruqola_TeamInfo)

using namespace Qt::Literals::StringLiterals;

TeamInfo::TeamInfo() = default;

void TeamInfo::parseTeamInfo(const QJsonObject &replyObject)
{
    mTeamId = replyObject.value("teamId"_L1).toString().toLatin1();
    mMainTeam = replyObject.value("teamMain"_L1).toBool(false);
    mAutoJoin = replyObject.value("teamDefault"_L1).toBool(false);
    mRoomsCount = replyObject.value("roomsCount"_L1).toInt(0);
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
    d.space() << "team id:" << t.teamId();
    d.space() << "is Main Team:" << t.mainTeam();
    d.space() << "autojoin:" << t.autoJoin();
    d.space() << "roomCount:" << t.roomsCount();
    return d;
}

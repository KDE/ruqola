/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamroom.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(TeamRoom, Ruqola_TeamRoom)

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
TeamRoom::TeamRoom() = default;

bool TeamRoom::operator==(const TeamRoom &other) const
{
    return mAutoJoin == other.autoJoin() && mName == other.name() && mFname == other.fname() && mIdentifier == other.identifier();
}

bool TeamRoom::autoJoin() const
{
    return mAutoJoin;
}

void TeamRoom::setAutoJoin(bool autoJoin)
{
    mAutoJoin = autoJoin;
}

QString TeamRoom::name() const
{
    return mName;
}

void TeamRoom::setName(const QString &name)
{
    mName = name;
}

QString TeamRoom::fname() const
{
    return mFname;
}

void TeamRoom::setFname(const QString &fname)
{
    mFname = fname;
}

void TeamRoom::parse(const QJsonObject &obj)
{
    mName = obj["name"_L1].toString();
    mFname = obj["fname"_L1].toString();
    mAutoJoin = obj["teamDefault"_L1].toBool(false);
    mIdentifier = obj["_id"_L1].toString().toLatin1();
}

QByteArray TeamRoom::identifier() const
{
    return mIdentifier;
}

void TeamRoom::setIdentifier(const QByteArray &identifier)
{
    mIdentifier = identifier;
}

QList<TeamRoom> TeamRoom::parseTeamRooms(const QJsonObject &obj)
{
    QList<TeamRoom> teamRooms;
    const QJsonArray rooms = obj.value("rooms"_L1).toArray();
    teamRooms.reserve(rooms.count());
    for (int i = 0, total = rooms.count(); i < total; ++i) {
        const QJsonObject r = rooms.at(i).toObject();
        TeamRoom teamRoom;
        teamRoom.parse(r);
        teamRooms.append(std::move(teamRoom));
    }
    return teamRooms;
}

QDebug operator<<(QDebug d, const TeamRoom &t)
{
    d.space() << "Name" << t.name();
    d.space() << "Fname" << t.fname();
    d.space() << "autojoin" << t.autoJoin();
    d.space() << "identifier" << t.identifier();
    return d;
}

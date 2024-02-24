/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamroom.h"

#include <QJsonArray>

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
    mName = obj[QLatin1String("name")].toString();
    mFname = obj[QLatin1String("fname")].toString();
    mAutoJoin = obj[QLatin1String("teamDefault")].toBool(false);
    mIdentifier = obj[QLatin1String("_id")].toString();
}

QString TeamRoom::identifier() const
{
    return mIdentifier;
}

void TeamRoom::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QList<TeamRoom> TeamRoom::parseTeamRooms(const QJsonObject &obj)
{
    QList<TeamRoom> teamRooms;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
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
    d << "Name " << t.name();
    d << "Fname : " << t.fname();
    d << "autojoin: " << t.autoJoin();
    d << "identifier: " << t.identifier();
    return d;
}

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

#include "teamroom.h"

#include <QJsonArray>

TeamRoom::TeamRoom()
{
}

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

QVector<TeamRoom> TeamRoom::parseTeamRooms(const QJsonObject &obj)
{
    QVector<TeamRoom> teamRooms;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
    teamRooms.reserve(rooms.count());
    for (int i = 0, total = rooms.count(); i < total; ++i) {
        const QJsonObject r = rooms.at(i).toObject();
        TeamRoom teamRoom;
        teamRoom.parse(r);
        teamRooms.append(teamRoom);
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

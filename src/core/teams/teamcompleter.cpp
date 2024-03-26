/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamcompleter.h"

TeamCompleter::TeamCompleter() = default;

bool TeamCompleter::operator==(const TeamCompleter &other) const
{
    return mName == other.name() && mFname == other.fname() && mTeamId == other.teamId();
}

QString TeamCompleter::name() const
{
    return mName;
}

void TeamCompleter::setName(const QString &name)
{
    mName = name;
}

QString TeamCompleter::fname() const
{
    return mFname;
}

void TeamCompleter::setFname(const QString &fname)
{
    mFname = fname;
}

void TeamCompleter::parse(const QJsonObject &obj)
{
    mName = obj[QLatin1StringView("name")].toString();
    mFname = obj[QLatin1StringView("fname")].toString();
    mTeamId = obj[QLatin1StringView("teamId")].toString().toLatin1();
    // TODO add room type too
}

QByteArray TeamCompleter::teamId() const
{
    return mTeamId;
}

void TeamCompleter::setTeamId(const QByteArray &identifier)
{
    mTeamId = identifier;
}

QDebug operator<<(QDebug d, const TeamCompleter &t)
{
    d << "Name " << t.name();
    d << "Fname : " << t.fname();
    d << "identifier: " << t.teamId();
    return d;
}

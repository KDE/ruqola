/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamcompleter.h"
#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(TeamCompleter, Ruqola_TeamCompleter)

using namespace Qt::Literals::StringLiterals;

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
    mName = obj["name"_L1].toString();
    mFname = obj["fname"_L1].toString();
    mTeamId = obj["teamId"_L1].toString().toLatin1();
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
    d.space() << "Name" << t.name();
    d.space() << "Fname:" << t.fname();
    d.space() << "identifier:" << t.teamId();
    return d;
}

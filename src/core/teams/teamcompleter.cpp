/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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
    // QJsonObject({"items":[{"_id":"zMHhMfsEPvKjgFuyE","fname":"ruqola-broadcast","name":"ruqola-broadcast","t":"p"},{"_id":"QMkvkiMyxKoEuJjnb","avatarETag":"MDRisL8NzZtsCdkYE","fname":"ruqola-test","name":"ruqola-test","t":"p"},
    mName = obj[QLatin1String("name")].toString();
    mFname = obj[QLatin1String("fname")].toString();
    mTeamId = obj[QLatin1String("teamId")].toString();
    // TODO add room type too
}

QString TeamCompleter::teamId() const
{
    return mTeamId;
}

void TeamCompleter::setTeamId(const QString &identifier)
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

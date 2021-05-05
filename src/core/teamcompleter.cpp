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

#include "teamcompleter.h"

TeamCompleter::TeamCompleter()
{
}

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

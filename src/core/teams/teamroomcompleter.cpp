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

#include "teamroomcompleter.h"

TeamRoomCompleter::TeamRoomCompleter() = default;

bool TeamRoomCompleter::operator==(const TeamRoomCompleter &other) const
{
    return mName == other.name() && mFname == other.fname() && mIdentifier == other.identifier();
}

QString TeamRoomCompleter::name() const
{
    return mName;
}

void TeamRoomCompleter::setName(const QString &name)
{
    mName = name;
}

QString TeamRoomCompleter::fname() const
{
    return mFname;
}

void TeamRoomCompleter::setFname(const QString &fname)
{
    mFname = fname;
}

void TeamRoomCompleter::parse(const QJsonObject &obj)
{
    // QJsonObject({"items":[{"_id":"zMHhMfsEPvKjgFuyE","fname":"ruqola-broadcast","name":"ruqola-broadcast","t":"p"},{"_id":"QMkvkiMyxKoEuJjnb","avatarETag":"MDRisL8NzZtsCdkYE","fname":"ruqola-test","name":"ruqola-test","t":"p"},
    mName = obj[QLatin1String("name")].toString();
    mFname = obj[QLatin1String("fname")].toString();
    mIdentifier = obj[QLatin1String("_id")].toString();
    // TODO add room type too
}

QString TeamRoomCompleter::identifier() const
{
    return mIdentifier;
}

void TeamRoomCompleter::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QDebug operator<<(QDebug d, const TeamRoomCompleter &t)
{
    d << "Name " << t.name();
    d << "Fname : " << t.fname();
    d << "identifier: " << t.identifier();
    return d;
}

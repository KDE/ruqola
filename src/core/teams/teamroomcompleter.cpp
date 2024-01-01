/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    // QJsonObject({"items":[{"_id":"zMHhMfsEPvKjgFuyE","fname":"ruqola-broadcast",
    // "name":"ruqola-broadcast","t":"p"},
    // {"_id":"QMkvkiMyxKoEuJjnb","avatarETag":"MDRisL8NzZtsCdkYE","fname":"ruqola-test","name":"ruqola-test","t":"p"},
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

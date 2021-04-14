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

TeamRoom::TeamRoom()
{
}

bool TeamRoom::operator==(const TeamRoom &other) const
{
    return mAutoJoin == other.autoJoin() && mName == other.name() && mFname == other.fname();
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

QDebug operator<<(QDebug d, const TeamRoom &t)
{
    d << "Name " << t.name();
    d << "Fname : " << t.fname();
    d << "autojoin: " << t.autoJoin();
    return d;
}

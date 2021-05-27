/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "adminrooms.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

AdminRooms::AdminRooms()
{
}

bool AdminRooms::isEmpty() const
{
    return mAdminRooms.isEmpty();
}

void AdminRooms::clear()
{
    mAdminRooms.clear();
}

int AdminRooms::count() const
{
    return mAdminRooms.count();
}

RoomInfo AdminRooms::at(int index) const
{
    return mAdminRooms.at(index);
}

void AdminRooms::parseMoreAdminRooms(const QJsonObject &fileAttachmentsObj)
{
    const int adminRoomsCount = fileAttachmentsObj[QStringLiteral("count")].toInt();
    mOffset = fileAttachmentsObj[QStringLiteral("offset")].toInt();
    mTotal = fileAttachmentsObj[QStringLiteral("total")].toInt();
    parseListAdminRooms(fileAttachmentsObj);
    mAdminRoomsCount += adminRoomsCount;
}

void AdminRooms::parseListAdminRooms(const QJsonObject &adminRoomsObj)
{
    const QJsonArray adminRoomsArray = adminRoomsObj[QStringLiteral("rooms")].toArray();
    mAdminRooms.reserve(mAdminRooms.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject adminRoomObject = current.toObject();
            RoomInfo m;
            m.parseAdminRoom(adminRoomObject);
            mAdminRooms.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing admin Rooms" << current;
        }
    }
}

int AdminRooms::adminRoomsCount() const
{
    return mAdminRoomsCount;
}

void AdminRooms::setAdminRoomsCount(int adminroomsCount)
{
    mAdminRoomsCount = adminroomsCount;
}

QVector<RoomInfo> AdminRooms::adminRooms() const
{
    return mAdminRooms;
}

void AdminRooms::setAdminRooms(const QVector<RoomInfo> &commands)
{
    mAdminRooms = commands;
}

void AdminRooms::parseAdminRooms(const QJsonObject &commandsObj)
{
    mAdminRoomsCount = commandsObj[QStringLiteral("count")].toInt();
    mOffset = commandsObj[QStringLiteral("offset")].toInt();
    mTotal = commandsObj[QStringLiteral("total")].toInt();
    mAdminRooms.clear();
    parseListAdminRooms(commandsObj);
}

int AdminRooms::offset() const
{
    return mOffset;
}

void AdminRooms::setOffset(int offset)
{
    mOffset = offset;
}

int AdminRooms::total() const
{
    return mTotal;
}

void AdminRooms::setTotal(int total)
{
    mTotal = total;
}

QDebug operator<<(QDebug d, const AdminRooms &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "adminRoomsCount " << t.adminRoomsCount() << "\n";
    for (int i = 0, total = t.adminRooms().count(); i < total; ++i) {
        d << t.adminRooms().at(i) << "\n";
    }
    return d;
}

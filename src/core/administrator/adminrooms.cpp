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
    return mRooms.isEmpty();
}

void AdminRooms::clear()
{
    mRooms.clear();
}

int AdminRooms::count() const
{
    return mRooms.count();
}

RoomInfo AdminRooms::at(int index) const
{
    return mRooms.at(index);
}

void AdminRooms::parseMoreRooms(const QJsonObject &fileAttachmentsObj)
{
    const int adminRoomsCount = fileAttachmentsObj[QStringLiteral("count")].toInt();
    mOffset = fileAttachmentsObj[QStringLiteral("offset")].toInt();
    mTotal = fileAttachmentsObj[QStringLiteral("total")].toInt();
    parseListRooms(fileAttachmentsObj);
    mRoomsCount += adminRoomsCount;
}

void AdminRooms::parseListRooms(const QJsonObject &adminRoomsObj)
{
    const QJsonArray adminRoomsArray = adminRoomsObj[QStringLiteral("rooms")].toArray();
    mRooms.reserve(mRooms.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject adminRoomObject = current.toObject();
            RoomInfo m;
            m.parseRoomInfo(adminRoomObject);
            mRooms.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing admin Rooms" << current;
        }
    }
}

int AdminRooms::roomsCount() const
{
    return mRoomsCount;
}

void AdminRooms::setRoomsCount(int adminroomsCount)
{
    mRoomsCount = adminroomsCount;
}

QVector<RoomInfo> AdminRooms::rooms() const
{
    return mRooms;
}

void AdminRooms::setRooms(const QVector<RoomInfo> &rooms)
{
    mRooms = rooms;
}

void AdminRooms::parseRooms(const QJsonObject &roomsObj)
{
    mRoomsCount = roomsObj[QStringLiteral("count")].toInt();
    mOffset = roomsObj[QStringLiteral("offset")].toInt();
    mTotal = roomsObj[QStringLiteral("total")].toInt();
    mRooms.clear();
    parseListRooms(roomsObj);
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
    d << "adminRoomsCount " << t.roomsCount() << "\n";
    for (int i = 0, total = t.rooms().count(); i < total; ++i) {
        d << t.rooms().at(i) << "\n";
    }
    return d;
}

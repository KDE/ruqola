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

#include "roominfo/roomsinfo.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

RoomsInfo::RoomsInfo() = default;

bool RoomsInfo::isEmpty() const
{
    return mRooms.isEmpty();
}

void RoomsInfo::clear()
{
    mRooms.clear();
}

int RoomsInfo::count() const
{
    return mRooms.count();
}

RoomInfo RoomsInfo::at(int index) const
{
    return mRooms.at(index);
}

void RoomsInfo::parseMoreRooms(const QJsonObject &obj, RoomsInfo::ParseType type)
{
    const int adminRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    parseListRooms(obj, type);
    mRoomsCount += adminRoomsCount;
}

void RoomsInfo::parseListRooms(const QJsonObject &adminRoomsObj, RoomsInfo::ParseType type)
{
    QString jsonKeyType;
    switch (type) {
    case Administrator:
        jsonKeyType = QStringLiteral("rooms");
        break;
    case Directory:
        jsonKeyType = QStringLiteral("result");
        break;
    }

    const QJsonArray adminRoomsArray = adminRoomsObj[jsonKeyType].toArray();
    mRooms.reserve(mRooms.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject adminRoomObject = current.toObject();
            RoomInfo m;
            m.parseRoomInfo(adminRoomObject);
            mRooms.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing Rooms" << current;
        }
    }
}

int RoomsInfo::roomsCount() const
{
    return mRoomsCount;
}

void RoomsInfo::setRoomsCount(int count)
{
    mRoomsCount = count;
}

RoomInfo RoomsInfo::takeAt(int index)
{
    return mRooms.takeAt(index);
}

QVector<RoomInfo> RoomsInfo::rooms() const
{
    return mRooms;
}

void RoomsInfo::setRooms(const QVector<RoomInfo> &rooms)
{
    mRooms = rooms;
}

void RoomsInfo::parseRooms(const QJsonObject &obj, RoomsInfo::ParseType type)
{
    mRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    mRooms.clear();
    parseListRooms(obj, type);
}

int RoomsInfo::offset() const
{
    return mOffset;
}

void RoomsInfo::setOffset(int offset)
{
    mOffset = offset;
}

int RoomsInfo::total() const
{
    return mTotal;
}

void RoomsInfo::setTotal(int total)
{
    mTotal = total;
}

QDebug operator<<(QDebug d, const RoomsInfo &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "roomsCount " << t.roomsCount() << "\n";
    for (int i = 0, total = t.rooms().count(); i < total; ++i) {
        d << t.rooms().at(i) << "\n";
    }
    return d;
}

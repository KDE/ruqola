/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roominfo/roomsinfo.h"

#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
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
    if (index < 0 || index >= mRooms.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mRooms.at(index);
}

void RoomsInfo::parseMoreRooms(const QJsonObject &obj, RoomsInfo::ParseType type)
{
    const int adminRoomsCount = obj["count"_L1].toInt();
    mOffset = obj["offset"_L1].toInt();
    mTotal = obj["total"_L1].toInt();
    parseListRooms(obj, type);
    mRoomsCount += adminRoomsCount;
}

void RoomsInfo::parseListRooms(const QJsonObject &adminRoomsObj, RoomsInfo::ParseType type)
{
    QString jsonKeyType;
    switch (type) {
    case ParseType::Administrator:
        jsonKeyType = u"rooms"_s;
        break;
    case ParseType::Directory:
        jsonKeyType = u"result"_s;
        break;
    }

    const QJsonArray adminRoomsArray = adminRoomsObj[jsonKeyType].toArray();
    mRooms.reserve(mRooms.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject adminRoomObject = current.toObject();
            RoomInfo m;
            m.parseRoomInfo(adminRoomObject);
            mRooms.append(std::move(m));
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

void RoomsInfo::insertRoom(int index, const RoomInfo &room)
{
    mRooms.insert(index, room);
}

QList<RoomInfo> RoomsInfo::rooms() const
{
    return mRooms;
}

void RoomsInfo::setRooms(const QList<RoomInfo> &rooms)
{
    mRooms = rooms;
}

void RoomsInfo::parseRooms(const QJsonObject &obj, RoomsInfo::ParseType type)
{
    mRoomsCount = obj["count"_L1].toInt();
    mOffset = obj["offset"_L1].toInt();
    mTotal = obj["total"_L1].toInt();
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
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "roomsCount" << t.roomsCount() << "\n";
    for (int i = 0, total = t.rooms().count(); i < total; ++i) {
        d.space() << t.rooms().at(i) << "\n";
    }
    return d;
}

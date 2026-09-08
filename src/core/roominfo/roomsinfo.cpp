/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsinfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(RoomsInfo, Ruqola_RoomsInfo)

using namespace Qt::Literals::StringLiterals;

namespace
{
// The administrator and the directory endpoints return the same rooms under a different key.
[[nodiscard]] QLatin1StringView arrayKey(RoomsInfo::ParseType type)
{
    switch (type) {
    case RoomsInfo::ParseType::Administrator:
        return "rooms"_L1;
    case RoomsInfo::ParseType::Directory:
        return "result"_L1;
    }
    return {};
}
}

void RoomsInfo::parseRooms(const QJsonObject &obj, ParseType type)
{
    parseFirstPageCounters(obj);
    parseElements(obj, arrayKey(type));
}

void RoomsInfo::parseMoreRooms(const QJsonObject &obj, ParseType type)
{
    parseNextPageCounters(obj);
    parseElements(obj, arrayKey(type));
}

void RoomsInfo::insertRoom(int index, const RoomInfo &room)
{
    mList.insert(index, room);
}

QDebug operator<<(QDebug d, const RoomsInfo &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "roomsCount" << t.loadedCount() << "\n";
    for (const RoomInfo &room : t.list()) {
        d.space() << room << "\n";
    }
    return d;
}

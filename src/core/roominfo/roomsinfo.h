/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
#include "roominfo.h"
class QDebug;

class LIBRUQOLACORE_EXPORT RoomsInfo : public PaginatedInfoList<RoomInfo, &RoomInfo::parseRoomInfo>
{
public:
    enum class ParseType : uint8_t {
        Administrator,
        Directory,
    };

    void parseRooms(const QJsonObject &obj, ParseType type);
    void parseMoreRooms(const QJsonObject &obj, ParseType type);

    void insertRoom(int index, const RoomInfo &room);
};

QT_DECL_METATYPE_EXTERN_TAGGED(RoomsInfo, Ruqola_RoomsInfo, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RoomsInfo &t);

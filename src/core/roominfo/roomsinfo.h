/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "roominfo.h"
#include <QDebug>
#include <QVector>
class LIBRUQOLACORE_EXPORT RoomsInfo
{
public:
    enum ParseType {
        Administrator,
        Directory,
    };
    RoomsInfo();

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] RoomInfo at(int index) const;

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    void parseRooms(const QJsonObject &obj, RoomsInfo::ParseType type);
    void parseMoreRooms(const QJsonObject &obj, RoomsInfo::ParseType type);

    [[nodiscard]] QVector<RoomInfo> rooms() const;
    void setRooms(const QVector<RoomInfo> &rooms);

    [[nodiscard]] int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

    RoomInfo takeAt(int index);
    void insertRoom(int index, const RoomInfo &room);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListRooms(const QJsonObject &commandsObj, ParseType type);
    QVector<RoomInfo> mRooms;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(RoomsInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RoomsInfo &t);

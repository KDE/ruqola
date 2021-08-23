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

#pragma once

#include "libruqola_private_export.h"
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

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT RoomInfo at(int index) const;

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    void parseRooms(const QJsonObject &obj, RoomsInfo::ParseType type);
    void parseMoreRooms(const QJsonObject &obj, RoomsInfo::ParseType type);

    Q_REQUIRED_RESULT QVector<RoomInfo> rooms() const;
    void setRooms(const QVector<RoomInfo> &rooms);

    Q_REQUIRED_RESULT int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

    Q_REQUIRED_RESULT RoomInfo takeAt(int index);

private:
    void parseListRooms(const QJsonObject &commandsObj, ParseType type);
    QVector<RoomInfo> mRooms;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(RoomsInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RoomsInfo &t);

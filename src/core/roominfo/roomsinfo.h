/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

    RoomInfo takeAt(int index);

private:
    void parseListRooms(const QJsonObject &commandsObj, ParseType type);
    QVector<RoomInfo> mRooms;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(RoomsInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const RoomsInfo &t);

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

#include "customsoundinfo.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QVector>
class LIBRUQOLACORE_EXPORT CustomSoundsInfo
{
public:
    enum ParseType {
        Administrator,
        Directory,
    };
    CustomSoundsInfo();

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT CustomSoundInfo at(int index) const;

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    void parseRooms(const QJsonObject &obj);
    void parseMoreRooms(const QJsonObject &obj);

    Q_REQUIRED_RESULT int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

    const QVector<CustomSoundInfo> &customSoundInfos() const;
    void setCustomSoundInfos(const QVector<CustomSoundInfo> &newCustomSoundInfos);

private:
    void parseListRooms(const QJsonObject &commandsObj);
    QVector<CustomSoundInfo> mCustomSoundInfos;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(CustomSoundsInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomSoundsInfo &t);

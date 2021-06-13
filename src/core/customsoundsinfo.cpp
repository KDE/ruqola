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

#include "customsoundsinfo.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

CustomSoundsInfo::CustomSoundsInfo()
{
}

bool CustomSoundsInfo::isEmpty() const
{
    return mCustomSoundInfos.isEmpty();
}

void CustomSoundsInfo::clear()
{
    mCustomSoundInfos.clear();
}

int CustomSoundsInfo::count() const
{
    return mCustomSoundInfos.count();
}

CustomSoundInfo CustomSoundsInfo::at(int index) const
{
    return mCustomSoundInfos.at(index);
}

void CustomSoundsInfo::parseMoreCustomSounds(const QJsonObject &obj)
{
    const int adminRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    parseListCustomSound(obj);
    mRoomsCount += adminRoomsCount;
}

void CustomSoundsInfo::parseListCustomSound(const QJsonObject &adminRoomsObj)
{
    QString jsonKeyType;
    const QJsonArray adminRoomsArray = adminRoomsObj[jsonKeyType].toArray();
    mCustomSoundInfos.reserve(mCustomSoundInfos.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject adminRoomObject = current.toObject();
            CustomSoundInfo m;
            m.parseCustomSoundInfo(adminRoomObject);
            mCustomSoundInfos.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing Rooms" << current;
        }
    }
}

const QVector<CustomSoundInfo> &CustomSoundsInfo::customSoundInfos() const
{
    return mCustomSoundInfos;
}

void CustomSoundsInfo::setCustomSoundInfos(const QVector<CustomSoundInfo> &newCustomSoundInfos)
{
    mCustomSoundInfos = newCustomSoundInfos;
}

int CustomSoundsInfo::roomsCount() const
{
    return mRoomsCount;
}

void CustomSoundsInfo::setRoomsCount(int count)
{
    mRoomsCount = count;
}

void CustomSoundsInfo::parseCustomSounds(const QJsonObject &obj)
{
    mRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    mCustomSoundInfos.clear();
    parseListCustomSound(obj);
}

int CustomSoundsInfo::offset() const
{
    return mOffset;
}

void CustomSoundsInfo::setOffset(int offset)
{
    mOffset = offset;
}

int CustomSoundsInfo::total() const
{
    return mTotal;
}

void CustomSoundsInfo::setTotal(int total)
{
    mTotal = total;
}

QDebug operator<<(QDebug d, const CustomSoundsInfo &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "roomsCount " << t.roomsCount() << "\n";
    for (int i = 0, total = t.customSoundInfos().count(); i < total; ++i) {
        d << t.customSoundInfos().at(i) << "\n";
    }
    return d;
}

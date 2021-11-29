/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "customsoundsinfo.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

CustomSoundsInfo::CustomSoundsInfo() = default;

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

void CustomSoundsInfo::parseListCustomSound(const QJsonObject &obj)
{
    const QJsonArray adminRoomsArray = obj[QLatin1String("sounds")].toArray();
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

CustomSoundInfo CustomSoundsInfo::takeAt(int index)
{
    return mCustomSoundInfos.takeAt(index);
}

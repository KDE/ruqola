/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    if (index < 0 || index > mCustomSoundInfos.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mCustomSoundInfos.at(index);
}

void CustomSoundsInfo::parseMoreCustomSounds(const QJsonObject &obj)
{
    const int adminRoomsCount = obj[QLatin1String("count")].toInt();
    mOffset = obj[QLatin1String("offset")].toInt();
    mTotal = obj[QLatin1String("total")].toInt();
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
            mCustomSoundInfos.append(std::move(m));
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
    mRoomsCount = obj[QLatin1String("count")].toInt();
    mOffset = obj[QLatin1String("offset")].toInt();
    mTotal = obj[QLatin1String("total")].toInt();
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
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "roomsCount" << t.roomsCount() << "\n";
    for (int i = 0, total = t.customSoundInfos().count(); i < total; ++i) {
        d.space() << t.customSoundInfos().at(i) << "\n";
    }
    return d;
}

CustomSoundInfo CustomSoundsInfo::takeAt(int index)
{
    return mCustomSoundInfos.takeAt(index);
}

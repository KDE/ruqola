/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customemojisinfo.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

CustomEmojisInfo::CustomEmojisInfo() = default;

bool CustomEmojisInfo::isEmpty() const
{
    return mCustomEmojiInfos.isEmpty();
}

void CustomEmojisInfo::clear()
{
    mCustomEmojiInfos.clear();
}

int CustomEmojisInfo::count() const
{
    return mCustomEmojiInfos.count();
}

CustomEmoji CustomEmojisInfo::at(int index) const
{
    if (index < 0 || index > mCustomEmojiInfos.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }

    return mCustomEmojiInfos.at(index);
}

void CustomEmojisInfo::parseMoreCustomEmojis(const QJsonObject &obj)
{
    const int adminRoomsCount = obj[QLatin1String("count")].toInt();
    mOffset = obj[QLatin1String("offset")].toInt();
    mTotal = obj[QLatin1String("total")].toInt();
    parseListCustomEmoji(obj);
    mRoomsCount += adminRoomsCount;
}

void CustomEmojisInfo::parseListCustomEmoji(const QJsonObject &obj)
{
    const QJsonArray adminRoomsArray = obj[QLatin1String("emojis")].toArray();
    mCustomEmojiInfos.reserve(mCustomEmojiInfos.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject adminRoomObject = current.toObject();
            CustomEmoji m;
            m.parseEmoji(adminRoomObject);
            mCustomEmojiInfos.append(std::move(m));
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing Rooms" << current;
        }
    }
}

const QVector<CustomEmoji> &CustomEmojisInfo::customSoundInfos() const
{
    return mCustomEmojiInfos;
}

void CustomEmojisInfo::setCustomSoundInfos(const QVector<CustomEmoji> &newCustomSoundInfos)
{
    mCustomEmojiInfos = newCustomSoundInfos;
}

int CustomEmojisInfo::roomsCount() const
{
    return mRoomsCount;
}

void CustomEmojisInfo::setRoomsCount(int count)
{
    mRoomsCount = count;
}

void CustomEmojisInfo::parseCustomEmojis(const QJsonObject &obj)
{
    mRoomsCount = obj[QLatin1String("count")].toInt();
    mOffset = obj[QLatin1String("offset")].toInt();
    mTotal = obj[QLatin1String("total")].toInt();
    mCustomEmojiInfos.clear();
    parseListCustomEmoji(obj);
}

int CustomEmojisInfo::offset() const
{
    return mOffset;
}

void CustomEmojisInfo::setOffset(int offset)
{
    mOffset = offset;
}

int CustomEmojisInfo::total() const
{
    return mTotal;
}

void CustomEmojisInfo::setTotal(int total)
{
    mTotal = total;
}

CustomEmoji CustomEmojisInfo::takeAt(int index)
{
    return mCustomEmojiInfos.takeAt(index);
}

QDebug operator<<(QDebug d, const CustomEmojisInfo &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "roomsCount" << t.roomsCount() << "\n";
    for (int i = 0, total = t.customSoundInfos().count(); i < total; ++i) {
        d.space() << t.customSoundInfos().at(i) << "\n";
    }
    return d;
}

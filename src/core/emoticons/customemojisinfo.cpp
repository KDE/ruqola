/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
    return mCustomEmojiInfos.at(index);
}

void CustomEmojisInfo::parseMoreCustomEmojis(const QJsonObject &obj)
{
    const int adminRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
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
            mCustomEmojiInfos.append(m);
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

void CustomEmojisInfo::parseCustomSounds(const QJsonObject &obj)
{
    mRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
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
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "roomsCount " << t.roomsCount() << "\n";
    for (int i = 0, total = t.customSoundInfos().count(); i < total; ++i) {
        d << t.customSoundInfos().at(i) << "\n";
    }
    return d;
}

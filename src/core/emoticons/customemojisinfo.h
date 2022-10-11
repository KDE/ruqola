/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customemoji.h"
#include "libruqolacore_export.h"
#include <QDebug>
#include <QVector>
class LIBRUQOLACORE_EXPORT CustomEmojisInfo
{
public:
    CustomEmojisInfo();

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT CustomEmoji at(int index) const;

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    void parseCustomSounds(const QJsonObject &obj);
    void parseMoreCustomEmojis(const QJsonObject &obj);

    Q_REQUIRED_RESULT int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

    const QVector<CustomEmoji> &customSoundInfos() const;
    void setCustomSoundInfos(const QVector<CustomEmoji> &newCustomSoundInfos);

    CustomEmoji takeAt(int index);

private:
    void parseListCustomEmoji(const QJsonObject &obj);
    QVector<CustomEmoji> mCustomEmojiInfos;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(CustomEmojisInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomEmojisInfo &t);

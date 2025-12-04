/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customemoji.h"
#include "libruqolacore_export.h"
#include <QList>
class LIBRUQOLACORE_EXPORT CustomEmojisInfo
{
public:
    CustomEmojisInfo();

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] CustomEmoji at(int index) const;

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    void parseCustomEmojis(const QJsonObject &obj);
    void parseMoreCustomEmojis(const QJsonObject &obj);

    [[nodiscard]] int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

    [[nodiscard]] const QList<CustomEmoji> &customSoundInfos() const;
    void setCustomSoundInfos(const QList<CustomEmoji> &newCustomSoundInfos);

    CustomEmoji takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListCustomEmoji(const QJsonObject &obj);
    QList<CustomEmoji> mCustomEmojiInfos;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

QT_DECL_METATYPE_EXTERN_TAGGED(CustomEmojisInfo, Ruqola_CustomEmojisInfo, LIBRUQOLACORE_EXPORT)
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomEmojisInfo &t);

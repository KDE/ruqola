/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customsoundinfo.h"
#include "libruqolacore_export.h"
#include <QList>
class LIBRUQOLACORE_EXPORT CustomSoundsInfo
{
public:
    enum class ParseType : uint8_t {
        Administrator,
        Directory,
    };
    CustomSoundsInfo();

    [[nodiscard]] bool isEmpty() const;
    void clear();
    [[nodiscard]] int count() const;
    [[nodiscard]] CustomSoundInfo at(int index) const;

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    void parseCustomSounds(const QJsonObject &obj);
    void parseMoreCustomSounds(const QJsonObject &obj);

    [[nodiscard]] int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

    [[nodiscard]] const QList<CustomSoundInfo> &customSoundInfos() const;
    void setCustomSoundInfos(const QList<CustomSoundInfo> &newCustomSoundInfos);

    CustomSoundInfo takeAt(int index);

private:
    LIBRUQOLACORE_NO_EXPORT void parseListCustomSound(const QJsonObject &obj);
    QList<CustomSoundInfo> mCustomSoundInfos;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

QT_DECL_METATYPE_EXTERN_TAGGED(CustomSoundsInfo, Ruqola_CustomSoundsInfo, LIBRUQOLACORE_EXPORT)
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomSoundsInfo &t);

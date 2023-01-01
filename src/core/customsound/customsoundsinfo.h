/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customsoundinfo.h"
#include "libruqolacore_export.h"
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

    void parseCustomSounds(const QJsonObject &obj);
    void parseMoreCustomSounds(const QJsonObject &obj);

    Q_REQUIRED_RESULT int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

    const QVector<CustomSoundInfo> &customSoundInfos() const;
    void setCustomSoundInfos(const QVector<CustomSoundInfo> &newCustomSoundInfos);

    CustomSoundInfo takeAt(int index);

private:
    void parseListCustomSound(const QJsonObject &obj);
    QVector<CustomSoundInfo> mCustomSoundInfos;
    int mRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(CustomSoundsInfo)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomSoundsInfo &t);

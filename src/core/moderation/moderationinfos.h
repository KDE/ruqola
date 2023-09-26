/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationinfo.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>
#include <QVector>

class LIBRUQOLACORE_EXPORT ModerationInfos
{
public:
    ModerationInfos();

    void clear();

    [[nodiscard]] int offset() const;
    void setOffset(int newOffset);

    [[nodiscard]] int total() const;
    void setTotal(int newTotal);

    [[nodiscard]] int moderationInfosCount() const;
    void setModerationInfosCount(int newModerationInfosCount);

    [[nodiscard]] const QVector<ModerationInfo> &moderationInfosList() const;
    void setModerationInfosList(const QVector<ModerationInfo> &newModerationInfosList);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    [[nodiscard]] ModerationInfo at(int index) const;

    void parseModerationInfos(const QJsonObject &moderationInfosObj);
    void parseMoreModerationInfos(const QJsonObject &moderationInfosObj);

    ModerationInfo takeAt(int index);

private:
    void parseModerationInfosObj(const QJsonObject &moderationInfosObj);
    QVector<ModerationInfo> mModerationInfosList;
    int mModerationInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ModerationInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationInfos &t);

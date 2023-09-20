/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int newOffset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int newTotal);

    Q_REQUIRED_RESULT int ModerationInfosCount() const;
    void setModerationInfosCount(int newModerationInfosCount);

    Q_REQUIRED_RESULT const QVector<ModerationInfo> &ModerationInfosList() const;
    void setModerationInfosList(const QVector<ModerationInfo> &newModerationInfosList);

    Q_REQUIRED_RESULT bool isEmpty() const;
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT ModerationInfo at(int index) const;

    void parseModerationInfos(const QJsonObject &discussionsObj);
    void parseMoreModerationInfos(const QJsonObject &ModerationInfosObj);

    ModerationInfo takeAt(int index);

private:
    void parseModerationInfosObj(const QJsonObject &discussionsObj);
    QVector<ModerationInfo> mModerationInfosList;
    int mModerationInfosCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};
Q_DECLARE_METATYPE(ModerationInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationInfos &t);

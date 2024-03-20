/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT ModerationReportedUserInfo
{
public:
    ModerationReportedUserInfo();
    ~ModerationReportedUserInfo() = default;
    [[nodiscard]] bool operator==(const ModerationReportedUserInfo &other) const;

    void parseModerationReportedUserInfo(const QJsonObject &replyObject);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 newTimeStamp);

    [[nodiscard]] int count() const;
    void setCount(int newCount);

private:
    qint64 mTimeStamp = -1;
    int mCount = 0;
};

Q_DECLARE_METATYPE(ModerationReportedUserInfo)
Q_DECLARE_TYPEINFO(ModerationReportedUserInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedUserInfo &t);

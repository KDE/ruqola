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

    void parseModerationReportedUserInfo(const QJsonObject &replyObject);

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 newTimeStamp);

private:
    qint64 mTimeStamp = -1;
};

Q_DECLARE_METATYPE(ModerationReportedUserInfo)
Q_DECLARE_TYPEINFO(ModerationReportedUserInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedUserInfo &t);

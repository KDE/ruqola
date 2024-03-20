/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreporteduserinfo.h"
#include "utils.h"

ModerationReportedUserInfo::ModerationReportedUserInfo() = default;

QDebug operator<<(QDebug d, const ModerationReportedUserInfo &t)
{
    d.space() << "count" << t.count();
    d.space() << "timeStamp" << t.timeStamp();
    return d;
}

bool ModerationReportedUserInfo::operator==(const ModerationReportedUserInfo &other) const
{
    return mCount == other.count() && mTimeStamp == other.timeStamp();
}

void ModerationReportedUserInfo::parseModerationReportedUserInfo(const QJsonObject &o)
{
    mCount = o[QLatin1StringView("count")].toInt();
    setTimeStamp(Utils::parseIsoDate(QStringLiteral("ts"), o));
    // TODO
}

qint64 ModerationReportedUserInfo::timeStamp() const
{
    return mTimeStamp;
}

void ModerationReportedUserInfo::setTimeStamp(qint64 newTimeStamp)
{
    mTimeStamp = newTimeStamp;
}

int ModerationReportedUserInfo::count() const
{
    return mCount;
}

void ModerationReportedUserInfo::setCount(int newCount)
{
    mCount = newCount;
}

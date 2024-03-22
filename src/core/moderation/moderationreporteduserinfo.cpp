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
    d.space() << "timeStamp" << t.createdAt();
    return d;
}

bool ModerationReportedUserInfo::operator==(const ModerationReportedUserInfo &other) const
{
    return mCount == other.count() && mCreatedAt == other.createdAt();
}

void ModerationReportedUserInfo::parseModerationReportedUserInfo(const QJsonObject &o)
{
    mCount = o[QLatin1StringView("count")].toInt();
    setCreatedAt(Utils::parseIsoDate(QStringLiteral("ts"), o));
    parseReportedUser(o[QLatin1StringView("reportedUser")].toObject());
}

void ModerationReportedUserInfo::parseReportedUser(const QJsonObject &o)
{
    // TODO
}

qint64 ModerationReportedUserInfo::createdAt() const
{
    return mCreatedAt;
}

void ModerationReportedUserInfo::setCreatedAt(qint64 newTimeStamp)
{
    mCreatedAt = newTimeStamp;
    if (mCreatedAt != -1) {
        QLocale l;
        mCreateAtDisplayDateTime = l.toString(QDateTime::fromMSecsSinceEpoch(mCreatedAt), QLocale::LongFormat);
    }
}

const QString &ModerationReportedUserInfo::createAtDisplayDateTime() const
{
    return mCreateAtDisplayDateTime;
}

int ModerationReportedUserInfo::count() const
{
    return mCount;
}

void ModerationReportedUserInfo::setCount(int newCount)
{
    mCount = newCount;
}

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
    d.space() << "username" << t.username();
    d.space() << "name" << t.name();
    d.space() << "identifier" << t.identifier();
    return d;
}

bool ModerationReportedUserInfo::operator==(const ModerationReportedUserInfo &other) const
{
    return mCount == other.count() && mCreatedAt == other.createdAt() && mUsername == other.username() && mName == other.name()
        && mIdentifier == other.identifier();
}

void ModerationReportedUserInfo::parseModerationReportedUserInfo(const QJsonObject &o)
{
    mCount = o[QLatin1StringView("count")].toInt();
    setCreatedAt(Utils::parseIsoDate(QStringLiteral("ts"), o));
    parseReportedUser(o[QLatin1StringView("reportedUser")].toObject());
}

void ModerationReportedUserInfo::parseReportedUser(const QJsonObject &o)
{
    mUsername = o[QLatin1StringView("username")].toString();
    mName = o[QLatin1StringView("name")].toString();
    mIdentifier = o[QLatin1StringView("_id")].toString().toLatin1();
    // TODO add emails!
}

QByteArray ModerationReportedUserInfo::identifier() const
{
    return mIdentifier;
}

void ModerationReportedUserInfo::setIdentifier(const QByteArray &newIdentifier)
{
    mIdentifier = newIdentifier;
}

QString ModerationReportedUserInfo::name() const
{
    return mName;
}

void ModerationReportedUserInfo::setName(const QString &newName)
{
    mName = newName;
}

QString ModerationReportedUserInfo::username() const
{
    return mUsername;
}

void ModerationReportedUserInfo::setUsername(const QString &newUsername)
{
    mUsername = newUsername;
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

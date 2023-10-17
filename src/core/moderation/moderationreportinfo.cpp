/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfo.h"
#include "utils.h"

#include <QLocale>

ModerationReportInfo::ModerationReportInfo() = default;

ModerationReportInfo::~ModerationReportInfo() = default;

bool ModerationReportInfo::operator==(const ModerationReportInfo &other) const
{
    return mDescription == other.description() && mReportIdentifier == other.reportIdentifier() && mTimeStamp == other.timeStamp();
}

void ModerationReportInfo::parseModerationReportInfo(const QJsonObject &o)
{
    mReportIdentifier = o[QLatin1String("_id")].toString();
    mDescription = o[QLatin1String("description")].toString();
    setTimeStamp(Utils::parseIsoDate(QStringLiteral("ts"), o));
    parseRoom(o[QLatin1String("room")].toObject());
    parseReportedBy(o[QLatin1String("reportedBy")].toObject());
    // TODO add more
    //{"reports":[{"_id":"6523e2465d66533fd6f85856","description":"test message report\n","reportedBy":{"_id":"H7Q9djXQ4zD9T2","username":"bla","name":"foo",
    // "createdAt":"2018-03-13T16:11:51.761Z"},"room":{"_id":"bBvCRnStXcG68zjna","name":"roomname","t":"p","fname":"roomname"},
    // "ts":"2023-10-09T11:21:42.135Z"}],"count":1,"offset":0,"total":1,"success":true}
    // TODO
}

qint64 ModerationReportInfo::timeStamp() const
{
    return mTimeStamp;
}

void ModerationReportInfo::setTimeStamp(qint64 newTimeStamp)
{
    mTimeStamp = newTimeStamp;
    QLocale l;
    mTimeStampDateTimeStr = l.toString(QDateTime::fromMSecsSinceEpoch(mTimeStamp), QLocale::LongFormat);
}

QString ModerationReportInfo::description() const
{
    return mDescription;
}

void ModerationReportInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QString ModerationReportInfo::reportIdentifier() const
{
    return mReportIdentifier;
}

void ModerationReportInfo::setReportIdentifier(const QString &newReportIdentifier)
{
    mReportIdentifier = newReportIdentifier;
}

void ModerationReportInfo::parseReportedBy(const QJsonObject &o)
{
    // TODO
}

void ModerationReportInfo::parseRoom(const QJsonObject &o)
{
    // TODO
}

QDebug operator<<(QDebug d, const ModerationReportInfo &t)
{
    d << "mDescription " << t.description();
    d << "mReportIdentifier " << t.reportIdentifier();
    d << "mTimeStamp " << t.timeStamp();
    return d;
}

#include "moc_moderationreportinfo.cpp"

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
    return mDescription == other.description() && mReportIdentifier == other.reportIdentifier() && mTimeStamp == other.timeStamp() && mRoomId == other.roomId()
        && mUserId == other.userId() && mUserName == other.userName();
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
}

QString ModerationReportInfo::timeStampDateTimeStr() const
{
    return mTimeStampDateTimeStr;
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
    mUserId = o[QLatin1String("_id")].toString();
    mUserName = o[QLatin1String("username")].toString();
    // TODO
}

QString ModerationReportInfo::userName() const
{
    return mUserName;
}

void ModerationReportInfo::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QString ModerationReportInfo::userId() const
{
    return mUserId;
}

void ModerationReportInfo::setUserId(const QString &newUserId)
{
    mUserId = newUserId;
}

QString ModerationReportInfo::roomId() const
{
    return mRoomId;
}

void ModerationReportInfo::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

void ModerationReportInfo::parseRoom(const QJsonObject &o)
{
    mRoomId = o[QLatin1String("_id")].toString();
    // TODO add more ? roomName etc ?
}

QDebug operator<<(QDebug d, const ModerationReportInfo &t)
{
    d.space() << "mDescription" << t.description();
    d.space() << "mReportIdentifier" << t.reportIdentifier();
    d.space() << "mTimeStamp" << t.timeStamp();
    d.space() << "mRoomId" << t.roomId();
    d.space() << "mUserId" << t.userId();
    d.space() << "mUserName" << t.userName();
    return d;
}

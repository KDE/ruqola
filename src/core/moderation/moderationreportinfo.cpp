/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfo.h"

ModerationReportInfo::ModerationReportInfo() = default;

ModerationReportInfo::~ModerationReportInfo() = default;

bool ModerationReportInfo::operator==(const ModerationReportInfo &other) const
{
    return mDescription == other.description() && mReportIdentifier == other.reportIdentifier();
}

void ModerationReportInfo::parseModerationReportInfo(const QJsonObject &o)
{
    mReportIdentifier = o[QLatin1String("o")].toString();
    mDescription = o[QLatin1String("description")].toString();
    // TODO add more
    //{"reports":[{"_id":"6523e2465d66533fd6f85856","description":"test message report\n","reportedBy":{"_id":"H7Q9djXQ4zD9T2","username":"bla","name":"foo",
    // "createdAt":"2018-03-13T16:11:51.761Z"},"room":{"_id":"bBvCRnStXcG68zjna","name":"roomname","t":"p","fname":"roomname"},
    // "ts":"2023-10-09T11:21:42.135Z"}],"count":1,"offset":0,"total":1,"success":true}
    // TODO
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

QDebug operator<<(QDebug d, const ModerationReportInfo &t)
{
    d << "mDescription " << t.description();
    d << "mReportIdentifier " << t.reportIdentifier();
    return d;
}

#include "moc_moderationreportinfo.cpp"

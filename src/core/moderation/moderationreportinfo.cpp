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

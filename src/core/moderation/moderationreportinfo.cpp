/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfo.h"

ModerationReportInfo::ModerationReportInfo() = default;

ModerationReportInfo::~ModerationReportInfo() = default;

bool ModerationReportInfo::operator==(const ModerationReportInfo &other) const
{
    // TODO implement
    return true;
}

QString ModerationReportInfo::description() const
{
    return mDescription;
}

void ModerationReportInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

#include "moc_moderationreportinfo.cpp"

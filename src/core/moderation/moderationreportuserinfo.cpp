/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfo.h"

ModerationReportUserInfo::ModerationReportUserInfo() = default;

ModerationReportUserInfo::~ModerationReportUserInfo() = default;

bool ModerationReportUserInfo::operator==(const ModerationReportUserInfo &other) const
{
    return other.description() == mDescription;
}

void ModerationReportUserInfo::parseModerationReportUserInfo(const QJsonObject &o)
{
    // TODO
}

QString ModerationReportUserInfo::description() const
{
    return mDescription;
}

void ModerationReportUserInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QDebug operator<<(QDebug d, const ModerationReportUserInfo &t)
{
    d << "description: " << t.description();
    return d;
}

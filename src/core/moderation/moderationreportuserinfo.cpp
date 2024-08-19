/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfo.h"

ModerationReportUserInfo::ModerationReportUserInfo() = default;

ModerationReportUserInfo::~ModerationReportUserInfo() = default;

bool ModerationReportUserInfo::operator==(const ModerationReportUserInfo &other) const
{
    // TODO
    return false;
}

QDebug operator<<(QDebug d, const ModerationReportUserInfo &t)
{
    // TODO
    return d;
}

/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreporteduserinfo.h"

ModerationReportedUserInfo::ModerationReportedUserInfo() = default;

QDebug operator<<(QDebug d, const ModerationReportedUserInfo &t)
{
    // TODO
    return d;
}

void ModerationReportedUserInfo::parseModerationReportedUserInfo(const QJsonObject &replyObject)
{
    // TODO
}

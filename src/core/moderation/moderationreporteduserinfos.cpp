/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreporteduserinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ModerationReportedUserInfos, Ruqola_ModerationReportedUserInfos)

using namespace Qt::Literals::StringLiterals;

void ModerationReportedUserInfos::parseModerationReportedUserInfos(const QJsonObject &obj)
{
    parseInfos(obj, "reports"_L1);
}

void ModerationReportedUserInfos::parseMoreModerationReportedUserInfos(const QJsonObject &obj)
{
    parseMoreInfos(obj, "reports"_L1);
}

QDebug operator<<(QDebug d, const ModerationReportedUserInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "ModerationReportedUserInfosCount" << t.loadedCount() << "\n";
    for (const ModerationReportedUserInfo &info : t.list()) {
        d.space() << info << "\n";
    }
    return d;
}

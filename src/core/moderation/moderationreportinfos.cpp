/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ModerationReportInfos, Ruqola_ModerationReportInfos)

using namespace Qt::Literals::StringLiterals;

void ModerationReportInfos::parseModerationReportInfos(const QJsonObject &obj)
{
    parseInfos(obj, "reports"_L1);
}

void ModerationReportInfos::parseMoreModerationReportInfos(const QJsonObject &obj)
{
    parseMoreInfos(obj, "reports"_L1);
}

QDebug operator<<(QDebug d, const ModerationReportInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "ModerationReportInfosCount" << t.loadedCount() << "\n";
    for (const ModerationReportInfo &info : t.list()) {
        d.space() << info << "\n";
    }
    return d;
}

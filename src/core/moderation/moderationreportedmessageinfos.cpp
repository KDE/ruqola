/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportedmessageinfos.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ModerationReportedMessageInfos, Ruqola_ModerationReportedMessageInfos)

using namespace Qt::Literals::StringLiterals;

void ModerationReportedMessageInfos::parseModerationInfos(const QJsonObject &obj)
{
    parseInfos(obj, "reports"_L1);
}

void ModerationReportedMessageInfos::parseMoreModerationInfos(const QJsonObject &obj)
{
    parseMoreInfos(obj, "reports"_L1);
}

QDebug operator<<(QDebug d, const ModerationReportedMessageInfos &t)
{
    d.space() << "total" << t.total();
    d.space() << "offset" << t.offset();
    d.space() << "ModerationInfosCount" << t.loadedCount() << "\n";
    for (const ModerationReportedMessageInfo &info : t.list()) {
        d.space() << info << "\n";
    }
    return d;
}

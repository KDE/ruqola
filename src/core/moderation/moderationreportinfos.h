/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationreportinfo.h"
#include "paginatedinfolist.h"
class QDebug;

class LIBRUQOLACORE_EXPORT ModerationReportInfos : public PaginatedInfoList<ModerationReportInfo, &ModerationReportInfo::parseModerationReportInfo>
{
public:
    void parseModerationReportInfos(const QJsonObject &obj);
    void parseMoreModerationReportInfos(const QJsonObject &obj);
};
QT_DECL_METATYPE_EXTERN_TAGGED(ModerationReportInfos, Ruqola_ModerationReportInfos, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportInfos &t);

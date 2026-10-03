/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationreportedmessageinfo.h"
#include "paginatedinfolist.h"
class QDebug;

class LIBRUQOLACORE_EXPORT ModerationReportedMessageInfos
    : public PaginatedInfoList<ModerationReportedMessageInfo, &ModerationReportedMessageInfo::parseModerationInfo>
{
public:
    void parseModerationInfos(const QJsonObject &obj);
    void parseMoreModerationInfos(const QJsonObject &obj);
};
QT_DECL_METATYPE_EXTERN_TAGGED(ModerationReportedMessageInfos, Ruqola_ModerationReportedMessageInfos, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedMessageInfos &t);

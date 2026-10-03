/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "moderationreporteduserinfo.h"
#include "paginatedinfolist.h"
class QDebug;

class LIBRUQOLACORE_EXPORT ModerationReportedUserInfos
    : public PaginatedInfoList<ModerationReportedUserInfo, &ModerationReportedUserInfo::parseModerationReportedUserInfo>
{
public:
    void parseModerationReportedUserInfos(const QJsonObject &obj);
    void parseMoreModerationReportedUserInfos(const QJsonObject &obj);
};
QT_DECL_METATYPE_EXTERN_TAGGED(ModerationReportedUserInfos, Ruqola_ModerationReportedUserInfos, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedUserInfos &t);

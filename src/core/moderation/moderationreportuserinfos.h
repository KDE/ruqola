/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include "moderationreportuserinfo.h"
#include "paginatedinfolist.h"
#include "user.h"
class QDebug;

class LIBRUQOLACORE_EXPORT ModerationReportUserInfos
    : public PaginatedInfoList<ModerationReportUserInfo, &ModerationReportUserInfo::parseModerationReportUserInfo>
{
public:
    [[nodiscard]] bool operator==(const ModerationReportUserInfos &other) const = default;

    [[nodiscard]] User user() const;
    void setUser(const User &newUser);

    void parseModerationReportUserInfos(const QJsonObject &obj);

private:
    User mUser;
};
QT_DECL_METATYPE_EXTERN_TAGGED(ModerationReportUserInfos, Ruqola_ModerationReportUserInfos, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportUserInfos &t);

/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customuserstatus.h"
#include "libruqolacore_export.h"
#include "paginatedinfolist.h"
class QDebug;

namespace CustomUserStatusesUtils
{
// CustomUserStatus::parseCustomStatus() has a second, defaulted argument, which a member pointer
// can't carry. This has to stay in the header: it is the base class's template argument below.
inline void parseRestApiStatus(CustomUserStatus &status, const QJsonObject &obj)
{
    status.parseCustomStatus(obj);
}
}

class LIBRUQOLACORE_EXPORT CustomUserStatuses : public PaginatedInfoList<CustomUserStatus, &CustomUserStatusesUtils::parseRestApiStatus>
{
public:
    void parseCustomUserStatuses(const QJsonObject &obj);

    void deleteCustomUserStatuses(const QJsonArray &replyArray);
    void updateCustomUserStatues(const QJsonArray &replyArray);
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const CustomUserStatuses &t);

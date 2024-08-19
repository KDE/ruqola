/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include "user.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT ModerationReportUserInfos
{
public:
    ModerationReportUserInfos();
    ~ModerationReportUserInfos();
};
Q_DECLARE_METATYPE(ModerationReportUserInfos)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportUserInfos &t);

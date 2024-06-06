/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
class LIBRUQOLACORE_EXPORT AppsMarketPlaceInfo
{
public:
    AppsMarketPlaceInfo();
    ~AppsMarketPlaceInfo();
};

Q_DECLARE_METATYPE(AppsMarketPlaceInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceInfo &t);

/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT AppsMarketPlaceInstalledInfo
{
public:
    AppsMarketPlaceInstalledInfo();
    ~AppsMarketPlaceInstalledInfo();

    void parseInstalledAppsMarketPlaceInfo(const QJsonObject &replyObject);
    [[nodiscard]] bool isPrivate() const;
    void setIsPrivate(bool newIsPrivate);

private:
    bool mIsPrivate = false;
};

Q_DECLARE_METATYPE(AppsMarketPlaceInstalledInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceInstalledInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceInstalledInfo &t);

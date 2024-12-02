/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinstalledinfo.h"
using namespace Qt::Literals::StringLiterals;
AppsMarketPlaceInstalledInfo::AppsMarketPlaceInstalledInfo() = default;

AppsMarketPlaceInstalledInfo::~AppsMarketPlaceInstalledInfo() = default;

QDebug operator<<(QDebug d, const AppsMarketPlaceInstalledInfo &t)
{
    // TODO
    return d;
}

void AppsMarketPlaceInstalledInfo::parseInstalledAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    // TODO
}

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
    d << "private " << t.isPrivate();
    return d;
}

void AppsMarketPlaceInstalledInfo::parseInstalledAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    mIsPrivate = replyObject["private"_L1].toBool();
    // TODO
}

bool AppsMarketPlaceInstalledInfo::isPrivate() const
{
    return mIsPrivate;
}

void AppsMarketPlaceInstalledInfo::setIsPrivate(bool newIsPrivate)
{
    mIsPrivate = newIsPrivate;
}

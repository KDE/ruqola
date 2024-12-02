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
    d << "name " << t.appName();
    d << "version " << t.version();
    return d;
}

void AppsMarketPlaceInstalledInfo::parseInstalledAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    mIsPrivate = replyObject["private"_L1].toBool();
    mAppName = replyObject["name"_L1].toString();
    mVersion = replyObject["version"_L1].toString();
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

QString AppsMarketPlaceInstalledInfo::appName() const
{
    return mAppName;
}

void AppsMarketPlaceInstalledInfo::setAppName(const QString &newAppName)
{
    mAppName = newAppName;
}

QString AppsMarketPlaceInstalledInfo::version() const
{
    return mVersion;
}

void AppsMarketPlaceInstalledInfo::setVersion(const QString &newVersion)
{
    mVersion = newVersion;
}

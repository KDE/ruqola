/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfo.h"

AppsCountInfo::AppsCountInfo() = default;

AppsCountInfo::~AppsCountInfo() = default;

void AppsCountInfo::parseCountInfo(const QJsonObject &replyObject)
{
    qDebug() << " replyObject " << replyObject;
    // {"maxMarketplaceApps":-1,"maxPrivateApps":-1,"success":true,"totalMarketplaceEnabled":1,"totalPrivateEnabled":1}
}

int AppsCountInfo::maxMarketplaceApps() const
{
    return mMaxMarketplaceApps;
}

void AppsCountInfo::setMaxMarketplaceApps(int newMaxMarketplaceApps)
{
    mMaxMarketplaceApps = newMaxMarketplaceApps;
}

int AppsCountInfo::maxPrivateApps() const
{
    return mMaxPrivateApps;
}

void AppsCountInfo::setMaxPrivateApps(int newMaxPrivateApps)
{
    mMaxPrivateApps = newMaxPrivateApps;
}

int AppsCountInfo::totalMarketplaceEnabled() const
{
    return mTotalMarketplaceEnabled;
}

void AppsCountInfo::setTotalMarketplaceEnabled(int newTotalMarketplaceEnabled)
{
    mTotalMarketplaceEnabled = newTotalMarketplaceEnabled;
}

int AppsCountInfo::totalPrivateEnabled() const
{
    return mTotalPrivateEnabled;
}

void AppsCountInfo::setTotalPrivateEnabled(int newTotalPrivateEnabled)
{
    mTotalPrivateEnabled = newTotalPrivateEnabled;
}

QDebug operator<<(QDebug d, const AppsCountInfo &t)
{
    d.space() << "maxMarketplaceApps" << t.maxMarketplaceApps();
    d.space() << "maxPrivateApps" << t.maxPrivateApps();
    d.space() << "totalMarketplaceEnabled" << t.totalMarketplaceEnabled();
    d.space() << "totalPrivateEnabled" << t.totalPrivateEnabled();
    return d;
}

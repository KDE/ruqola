/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfo.h"
#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(AppsCountInfo, Ruqola_AppsCountInfo)
using namespace Qt::Literals::StringLiterals;
AppsCountInfo::AppsCountInfo() = default;

AppsCountInfo::~AppsCountInfo() = default;

void AppsCountInfo::parseCountInfo(const QJsonObject &replyObject)
{
    mMaxMarketplaceApps = replyObject["maxMarketplaceApps"_L1].toInt();
    mMaxPrivateApps = replyObject["maxPrivateApps"_L1].toInt();
    mTotalMarketplaceEnabled = replyObject["totalMarketplaceEnabled"_L1].toInt();
    mTotalPrivateEnabled = replyObject["totalPrivateEnabled"_L1].toInt();
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

bool AppsCountInfo::operator==(const AppsCountInfo &other) const
{
    return mTotalPrivateEnabled == other.totalPrivateEnabled() && mTotalMarketplaceEnabled == other.totalMarketplaceEnabled()
        && mMaxMarketplaceApps == other.maxMarketplaceApps() && mMaxPrivateApps == other.maxPrivateApps();
}

bool AppsCountInfo::isValid() const
{
    return mMaxMarketplaceApps != -1 && mMaxPrivateApps != -1 && mTotalMarketplaceEnabled != -1 && mTotalPrivateEnabled != -1;
}

/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinfo.h"
using namespace Qt::Literals::StringLiterals;
AppsMarketPlaceInfo::AppsMarketPlaceInfo() = default;

AppsMarketPlaceInfo::~AppsMarketPlaceInfo() = default;

QDebug operator<<(QDebug d, const AppsMarketPlaceInfo &t)
{
    d << "appId " << t.appId();
    d << "isEnterpriseOnly " << t.isEnterpriseOnly();
    d << "appName " << t.appName();
    d << "categories " << t.categories();
    d << "documentationUrl " << t.documentationUrl();
    return d;
}

void AppsMarketPlaceInfo::parseAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    mAppId = replyObject["appId"_L1].toString().toLatin1();
    mIsEnterpriseOnly = replyObject["isEnterpriseOnly"_L1].toBool();
    mAppName = replyObject["name"_L1].toString();
    mDocumentationUrl = replyObject["documentationUrl"_L1].toString();
    // FIXME it's a QStringList mCategories = replyObject["categories"_L1].toString();
    // TODO
}

QByteArray AppsMarketPlaceInfo::appId() const
{
    return mAppId;
}

void AppsMarketPlaceInfo::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

bool AppsMarketPlaceInfo::isEnterpriseOnly() const
{
    return mIsEnterpriseOnly;
}

void AppsMarketPlaceInfo::setIsEnterpriseOnly(bool newIsEnterpriseOnly)
{
    mIsEnterpriseOnly = newIsEnterpriseOnly;
}

QString AppsMarketPlaceInfo::appName() const
{
    return mAppName;
}

void AppsMarketPlaceInfo::setAppName(const QString &newAppName)
{
    mAppName = newAppName;
}

QStringList AppsMarketPlaceInfo::categories() const
{
    return mCategories;
}

void AppsMarketPlaceInfo::setCategories(const QStringList &newCategories)
{
    mCategories = newCategories;
}

bool AppsMarketPlaceInfo::isValid() const
{
    // TODO
    return !mAppId.isEmpty();
}

QString AppsMarketPlaceInfo::documentationUrl() const
{
    return mDocumentationUrl;
}

void AppsMarketPlaceInfo::setDocumentationUrl(const QString &newDocumentationUrl)
{
    mDocumentationUrl = newDocumentationUrl;
}

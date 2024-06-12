/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinfo.h"

#include <QJsonArray>
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
    d << "purchaseType " << t.purchaseType();
    d << "description " << t.description();
    return d;
}

void AppsMarketPlaceInfo::parseAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    mAppId = replyObject["appId"_L1].toString().toLatin1();
    mIsEnterpriseOnly = replyObject["isEnterpriseOnly"_L1].toBool();
    mPurchaseType = replyObject["purchaseType"_L1].toString();
    mPrice = replyObject["price"_L1].toInt();

    const QJsonObject latestObj = replyObject["latest"_L1].toObject();
    const QJsonArray categoriesArray = latestObj["categories"_L1].toArray();

    QStringList lst;
    lst.reserve(categoriesArray.count());
    for (const QJsonValue &current : categoriesArray) {
        lst.append(current.toString());
    }
    mCategories = lst;

    mDescription = latestObj["description"_L1].toString();
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

QString AppsMarketPlaceInfo::purchaseType() const
{
    return mPurchaseType;
}

void AppsMarketPlaceInfo::setPurchaseType(const QString &newPurchaseType)
{
    mPurchaseType = newPurchaseType;
}

int AppsMarketPlaceInfo::price() const
{
    return mPrice;
}

void AppsMarketPlaceInfo::setPrice(int newPrice)
{
    mPrice = newPrice;
}

QString AppsMarketPlaceInfo::description() const
{
    return mDescription;
}

void AppsMarketPlaceInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QIcon AppsMarketPlaceInfo::icon() const
{
    return mIcon;
}

void AppsMarketPlaceInfo::setIcon(const QIcon &newIcon)
{
    mIcon = newIcon;
}

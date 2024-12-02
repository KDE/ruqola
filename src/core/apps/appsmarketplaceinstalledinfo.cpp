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
    d << "description " << t.description();
    d << "support " << t.support();
    d << "homePage " << t.homePage();
    d << "authorName " << t.authorName();
    return d;
}

bool AppsMarketPlaceInstalledInfo::operator==(const AppsMarketPlaceInstalledInfo &other) const
{
    return mAppName == other.mAppName && mVersion == other.mVersion && mIsPrivate == other.mIsPrivate && mDescription == other.mDescription
        && mSupport == other.mSupport && mHomePage == other.mHomePage && mAuthorName == other.mAuthorName;
}

void AppsMarketPlaceInstalledInfo::parseInstalledAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    mIsPrivate = replyObject["private"_L1].toBool();
    mAppName = replyObject["name"_L1].toString();
    mVersion = replyObject["version"_L1].toString();
    mDescription = replyObject["description"_L1].toString();

    parseAuthor(replyObject["author"_L1].toObject());
}

void AppsMarketPlaceInstalledInfo::parseAuthor(const QJsonObject &authorObject)
{
    mHomePage = authorObject["homepage"_L1].toString();
    mSupport = authorObject["support"_L1].toString();
    mAuthorName = authorObject["name"_L1].toString();
}

QString AppsMarketPlaceInstalledInfo::authorName() const
{
    return mAuthorName;
}

void AppsMarketPlaceInstalledInfo::setAuthorName(const QString &newAuthorName)
{
    mAuthorName = newAuthorName;
}

QString AppsMarketPlaceInstalledInfo::homePage() const
{
    return mHomePage;
}

void AppsMarketPlaceInstalledInfo::setHomePage(const QString &newHomePage)
{
    mHomePage = newHomePage;
}

QString AppsMarketPlaceInstalledInfo::support() const
{
    return mSupport;
}

void AppsMarketPlaceInstalledInfo::setSupport(const QString &newSupport)
{
    mSupport = newSupport;
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

QString AppsMarketPlaceInstalledInfo::description() const
{
    return mDescription;
}

void AppsMarketPlaceInstalledInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinstalledinfo.h"
#include "ruqola_debug.h"
using namespace Qt::Literals::StringLiterals;
AppsMarketPlaceInstalledInfo::AppsMarketPlaceInstalledInfo() = default;

AppsMarketPlaceInstalledInfo::~AppsMarketPlaceInstalledInfo() = default;

QDebug operator<<(QDebug d, const AppsMarketPlaceInstalledInfo &t)
{
    d.space() << "private" << t.isPrivate();
    d.space() << "name" << t.appName();
    d.space() << "version" << t.version();
    d.space() << "description" << t.description();
    d.space() << "support" << t.support();
    d.space() << "homePage" << t.homePage();
    d.space() << "authorName" << t.authorName();
    d.space() << "migrated" << t.migrated();
    d.space() << "id" << t.appId();
    d.space() << "status" << t.status();
    return d;
}

bool AppsMarketPlaceInstalledInfo::isValid() const
{
    return !mAppId.isEmpty();
}

bool AppsMarketPlaceInstalledInfo::operator==(const AppsMarketPlaceInstalledInfo &other) const
{
    return mAppName == other.mAppName && mVersion == other.mVersion && mIsPrivate == other.mIsPrivate && mDescription == other.mDescription
        && mSupport == other.mSupport && mHomePage == other.mHomePage && mAuthorName == other.mAuthorName && mMigrated == other.migrated()
        && mAppId == other.mAppId && mStatus == other.mStatus;
}

void AppsMarketPlaceInstalledInfo::changeApplicationStatus(const QString &str)
{
    mStatus = convertStatusFromString(str);
}

AppsMarketPlaceInstalledInfo::Status AppsMarketPlaceInstalledInfo::convertStatusFromString(const QString &str)
{
    if (str == "auto_enabled"_L1) {
        return AppsMarketPlaceInstalledInfo::Status::AutoEnabled;
    } else if (str == "manually_enabled"_L1) {
        return AppsMarketPlaceInstalledInfo::Status::ManuallyEnabled;
    } else if (str == "initialized"_L1) {
        return AppsMarketPlaceInstalledInfo::Status::Initialized;
    } else if (str == "manually_disabled"_L1) {
        return AppsMarketPlaceInstalledInfo::Status::ManuallyDisabled;
    } else if (str == "disabled"_L1) {
        return AppsMarketPlaceInstalledInfo::Status::Disabled;
    } else if (str == "constructed"_L1) {
        return AppsMarketPlaceInstalledInfo::Status::Constructed;
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown status type " << str;
        return AppsMarketPlaceInstalledInfo::Status::Unknown;
    }
}

QPixmap AppsMarketPlaceInstalledInfo::pixmap() const
{
    return mPixmap;
}

void AppsMarketPlaceInstalledInfo::parseInstalledAppsMarketPlaceInfo(const QJsonObject &replyObject)
{
    mIsPrivate = replyObject["private"_L1].toBool();
    mAppName = replyObject["name"_L1].toString();
    mVersion = replyObject["version"_L1].toString();
    mDescription = replyObject["description"_L1].toString();
    mMigrated = replyObject["migrated"_L1].toBool();
    mAppId = replyObject["id"_L1].toString().toLatin1();

    parseAuthor(replyObject["author"_L1].toObject());
    mStatus = convertStatusFromString(replyObject["status"_L1].toString());

    QByteArray baImageBase64 = replyObject["iconFileContent"_L1].toString().toLatin1();
    // We need only image info
    baImageBase64.replace("data:image/png;base64,", "");
    if (!baImageBase64.isEmpty() && !mPixmap.loadFromData(QByteArray::fromBase64(baImageBase64), "PNG")) {
        qCWarning(RUQOLA_LOG) << "Impossible to load pixmap: " << baImageBase64;
    }
}

void AppsMarketPlaceInstalledInfo::parseAuthor(const QJsonObject &authorObject)
{
    mHomePage = authorObject["homepage"_L1].toString();
    mSupport = authorObject["support"_L1].toString();
    mAuthorName = authorObject["name"_L1].toString();
}

AppsMarketPlaceInstalledInfo::Status AppsMarketPlaceInstalledInfo::status() const
{
    return mStatus;
}

void AppsMarketPlaceInstalledInfo::setStatus(Status newStatus)
{
    mStatus = newStatus;
}

QByteArray AppsMarketPlaceInstalledInfo::appId() const
{
    return mAppId;
}

void AppsMarketPlaceInstalledInfo::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

bool AppsMarketPlaceInstalledInfo::migrated() const
{
    return mMigrated;
}

void AppsMarketPlaceInstalledInfo::setMigrated(bool newMigrated)
{
    mMigrated = newMigrated;
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

#include "moc_appsmarketplaceinstalledinfo.cpp"

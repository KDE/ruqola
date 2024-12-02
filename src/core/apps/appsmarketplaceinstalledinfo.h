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

    [[nodiscard]] bool operator==(const AppsMarketPlaceInstalledInfo &other) const;

    void parseInstalledAppsMarketPlaceInfo(const QJsonObject &replyObject);

    [[nodiscard]] bool isPrivate() const;
    void setIsPrivate(bool newIsPrivate);

    [[nodiscard]] QString appName() const;
    void setAppName(const QString &newAppName);

    [[nodiscard]] QString version() const;
    void setVersion(const QString &newVersion);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] QString support() const;
    void setSupport(const QString &newSupport);

    [[nodiscard]] QString homePage() const;
    void setHomePage(const QString &newHomePage);

    [[nodiscard]] QString authorName() const;
    void setAuthorName(const QString &newAuthorName);

    [[nodiscard]] bool migrated() const;
    void setMigrated(bool newMigrated);

    [[nodiscard]] QString appId() const;
    void setAppId(const QString &newAppId);

private:
    LIBRUQOLACORE_NO_EXPORT void parseAuthor(const QJsonObject &authorObject);
    QString mAppName;
    QString mVersion;
    QString mDescription;
    QString mSupport;
    QString mHomePage;
    QString mAuthorName;
    QString mAppId;
    bool mIsPrivate = false;
    bool mMigrated = false;
};

Q_DECLARE_METATYPE(AppsMarketPlaceInstalledInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceInstalledInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceInstalledInfo &t);

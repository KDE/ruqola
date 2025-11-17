/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QPixmap>
class LIBRUQOLACORE_EXPORT AppsMarketPlaceInstalledInfo
{
    Q_GADGET
public:
    enum class Status : uint8_t {
        Unknown,
        AutoEnabled,
        Initialized,
        ManuallyEnabled,
        ManuallyDisabled,
        Disabled,
        Constructed,
    };
    Q_ENUM(Status);

    AppsMarketPlaceInstalledInfo();
    ~AppsMarketPlaceInstalledInfo();

    [[nodiscard]] bool isValid() const;

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

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] Status status() const;
    void setStatus(Status newStatus);

    [[nodiscard]] QPixmap pixmap() const;

    void changeApplicationStatus(const QString &str);

private:
    LIBRUQOLACORE_NO_EXPORT void parseAuthor(const QJsonObject &authorObject);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT AppsMarketPlaceInstalledInfo::Status convertStatusFromString(const QString &str);
    QPixmap mPixmap;
    QString mAppName;
    QString mVersion;
    QString mDescription;
    QString mSupport;
    QString mHomePage;
    QString mAuthorName;
    QByteArray mAppId;
    Status mStatus = Status::Unknown;
    bool mIsPrivate = false;
    bool mMigrated = false;
};
Q_DECLARE_METATYPE(AppsMarketPlaceInstalledInfo::Status)
Q_DECLARE_METATYPE(AppsMarketPlaceInstalledInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceInstalledInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceInstalledInfo &t);

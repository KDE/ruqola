/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QPixmap>
class LIBRUQOLACORE_EXPORT AppsMarketPlaceInfo
{
public:
    struct LIBRUQOLACORE_EXPORT PricePlan {
        [[nodiscard]] bool operator==(const PricePlan &other) const;

        enum Strategy {
            Unknown,
            Monthly,
            // More ?
        };

        [[nodiscard]] Strategy convertStringToStrategy(const QString &str) const;
        int price = -1;
        int trialDays = -1;
        Strategy strategy = Strategy::Unknown;
        bool enabled = false;
    };
    AppsMarketPlaceInfo();
    ~AppsMarketPlaceInfo();

    void parseAppsMarketPlaceInfo(const QJsonObject &replyObject);

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] bool isEnterpriseOnly() const;
    void setIsEnterpriseOnly(bool newIsEnterpriseOnly);

    [[nodiscard]] QString appName() const;
    void setAppName(const QString &newAppName);

    [[nodiscard]] QStringList categories() const;
    void setCategories(const QStringList &newCategories);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QString documentationUrl() const;
    void setDocumentationUrl(const QString &newDocumentationUrl);

    [[nodiscard]] QString purchaseType() const;
    void setPurchaseType(const QString &newPurchaseType);

    [[nodiscard]] int price() const;
    void setPrice(int newPrice);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &newDescription);

    [[nodiscard]] QPixmap pixmap() const;
    void setPixmap(const QPixmap &newIcon);

    [[nodiscard]] QString version() const;
    void setVersion(const QString &newVersion);

    [[nodiscard]] QString shortDescription() const;
    void setShortDescription(const QString &newShortDescription);

    [[nodiscard]] bool operator==(const AppsMarketPlaceInfo &other) const;

    [[nodiscard]] qint64 modifiedDate() const;
    void setModifiedDate(qint64 newModifiedDate);

    [[nodiscard]] QString applicationInformations() const;

    [[nodiscard]] bool isPaid() const;

    [[nodiscard]] QList<PricePlan> pricePlan() const;
    void setPricePlan(const QList<PricePlan> &newPricePlan);

    [[nodiscard]] bool isPrivate() const;
    void setIsPrivate(bool newIsPrivate);

    void parseInstalledApps(const QJsonObject &replyObject);

    [[nodiscard]] QString support() const;
    void setSupport(const QString &newSupport);

    [[nodiscard]] QString homePage() const;
    void setHomePage(const QString &newHomePage);

    [[nodiscard]] QString privacyPolicySummary() const;
    void setPrivacyPolicySummary(const QString &newPrivacyPolicySummary);

private:
    LIBRUQOLACORE_NO_EXPORT void parsePrincingPlan(const QJsonArray &array);
    LIBRUQOLACORE_NO_EXPORT void parseAuthor(const QJsonObject &authorObject);
    QList<PricePlan> mPricePlan;
    QStringList mCategories;
    QByteArray mAppId;
    QString mAppName;
    QString mDescription;
    QString mDocumentationUrl;
    QString mPurchaseType;
    QString mVersion;
    QString mShortDescription;
    QString mSupport;
    QString mHomePage;
    QPixmap mPixmap;
    QString mPrivacyPolicySummary;
    qint64 mModifiedDate = -1;
    int mPrice = 0;
    bool mIsPrivate = false;
    bool mIsEnterpriseOnly = false;
};

Q_DECLARE_METATYPE(AppsMarketPlaceInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceInfo &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceInfo::PricePlan &t);

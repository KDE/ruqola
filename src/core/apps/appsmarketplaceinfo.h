/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QIcon>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT AppsMarketPlaceInfo
{
public:
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

    [[nodiscard]] QIcon icon() const;
    void setIcon(const QIcon &newIcon);

private:
    QStringList mCategories;
    QByteArray mAppId;
    QString mAppName;
    QString mDescription;
    QString mDocumentationUrl;
    QString mPurchaseType;
    QIcon mIcon;
    int mPrice = 0;
    bool mIsEnterpriseOnly = false;
    // TODO icon
};

Q_DECLARE_METATYPE(AppsMarketPlaceInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceInfo &t);

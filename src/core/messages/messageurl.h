/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT MessageUrl
{
    Q_GADGET
public:
    MessageUrl();
    [[nodiscard]] bool operator==(const MessageUrl &other) const;
    static QJsonObject serialize(const MessageUrl &url);
    static MessageUrl deserialize(const QJsonObject &o);

    [[nodiscard]] QString url() const;
    void setUrl(const QString &url);

    [[nodiscard]] QString pageTitle() const;
    void setPageTitle(const QString &pageTitle);

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

    void parseUrl(const QJsonObject &url);

    [[nodiscard]] QString imageUrl() const;
    void setImageUrl(const QString &newImageUrl);

    [[nodiscard]] QString authorName() const;
    void setAuthorName(const QString &newAuthorName);

    [[nodiscard]] QString authorUrl() const;
    void setAuthorUrl(const QString &newAuthorUrl);

    [[nodiscard]] QString siteUrl() const;
    void setSiteUrl(const QString &newSiteUrl);

    [[nodiscard]] QString siteName() const;
    void setSiteName(const QString &newSiteName);

    [[nodiscard]] int imageHeight() const;
    void setImageHeight(int newImageHeight);

    [[nodiscard]] int imageWidth() const;
    void setImageWidth(int newImageWidth);

private:
    [[nodiscard]] static QStringList pageTitleElements();
    [[nodiscard]] static QStringList descriptionElements();
    [[nodiscard]] static QStringList imageUrlElements();
    [[nodiscard]] static QStringList siteUrlElements();
    [[nodiscard]] static QStringList siteNameElements();
    [[nodiscard]] static QStringList imageHeightElements();
    [[nodiscard]] static QStringList imageWidthElements();
    QString mUrl;
    QString mPageTitle;
    QString mDescription;
    QString mImageUrl;
    QString mAuthorName;
    QString mAuthorUrl;
    QString mSiteUrl;
    QString mSiteName;
    int mImageHeight = -1;
    int mImageWidth = -1;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageUrl &t);
Q_DECLARE_TYPEINFO(MessageUrl, Q_MOVABLE_TYPE);

/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QMetaType>
class QJsonObject;
class LIBRUQOLACORE_EXPORT MessageUrl
{
    Q_GADGET
public:
    MessageUrl();

    enum class ContentType : uint8_t {
        None,
        Image,
        ImageAnimated,
        Audio,
        Video,
    };
    Q_ENUM(ContentType)

    [[nodiscard]] bool operator==(const MessageUrl &other) const;
    [[nodiscard]] static QJsonObject serialize(const MessageUrl &url);
    [[nodiscard]] static MessageUrl deserialize(const QJsonObject &o);

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

    [[nodiscard]] QByteArray urlId() const;
    void setUrlId(const QByteArray &newUrlId);

    [[nodiscard]] bool showPreview() const;
    void setShowPreview(bool newShowPreview);

    [[nodiscard]] bool hasPreviewUrl() const;

    [[nodiscard]] QString htmlDescription() const;

    [[nodiscard]] bool hasHtmlDescription() const;

    [[nodiscard]] QString buildImageUrl() const;

    void generateMessageUrlInfo();

    [[nodiscard]] ContentType contentType() const;
    void setContentType(ContentType newContentType);

    [[nodiscard]] static QString contentTypeEnumToString(ContentType type);
    [[nodiscard]] static MessageUrl::ContentType stringToContentTypeEnum(const QString &str);

    [[nodiscard]] MessageUrl::ContentType parseHeaderContentType(const QString &typeHeader) const;

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QStringList pageTitleElements();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QStringList descriptionElements();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QStringList imageUrlElements();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QStringList siteUrlElements();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QStringList siteNameElements();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QStringList imageHeightElements();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QStringList imageWidthElements();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT static QString cleanText(const QString &str);
    LIBRUQOLACORE_NO_EXPORT void generateHtmlDescription();
    LIBRUQOLACORE_NO_EXPORT void generateImageUrl();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool hasPreviewContentType() const;

    QString mUrl;
    QString mPageTitle;
    QString mDescription;
    QString mImageUrl;
    QString mAuthorName;
    QString mAuthorUrl;
    QString mSiteUrl;
    QString mSiteName;
    QByteArray mUrlId;
    QString mHtmlDescription;
    QString mImageBuildUrl;
    int mImageHeight = -1;
    int mImageWidth = -1;
    bool mShowPreview = true;
    ContentType mContentType = ContentType::None;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const MessageUrl &t);
Q_DECLARE_TYPEINFO(MessageUrl, Q_RELOCATABLE_TYPE);

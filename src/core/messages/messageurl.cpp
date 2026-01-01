/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurl.h"

#include "ruqola_message_url_debug.h"
#include "ruqolaglobalconfig.h"

#include <QJsonObject>
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;
MessageUrl::MessageUrl()
{
    mShowPreview = RuqolaGlobalConfig::self()->showPreviewUrl();
}

QStringList MessageUrl::pageTitleElements()
{
    return {u"ogTitle"_s, u"twitterTitle"_s, u"title"_s, QStringLiteral("pageTitle"), QStringLiteral("oembedTitle")};
}

QStringList MessageUrl::descriptionElements()
{
    return {u"ogDescription"_s, u"twitterDescription"_s, u"description"_s};
}

QStringList MessageUrl::imageUrlElements()
{
    return {u"ogImage"_s, u"twitterImage"_s, u"msapplicationTileImage"_s, QStringLiteral("oembedThumbnailUrl")};
}

QStringList MessageUrl::siteUrlElements()
{
    return {u"ogUrl"_s, u"oembedProviderUrl"_s};
}

QStringList MessageUrl::siteNameElements()
{
    return {u"ogSiteName"_s, u"oembedProviderName"_s};
}

QStringList MessageUrl::imageHeightElements()
{
    return {u"ogImageHeight"_s, u"oembedHeight"_s, u"oembedThumbnailHeight"_s};
}

QStringList MessageUrl::imageWidthElements()
{
    return {u"ogImageWidth"_s, u"oembedWidth"_s, u"oembedThumbnailWidth"_s};
}

bool MessageUrl::showPreview() const
{
    return mShowPreview;
}

void MessageUrl::setShowPreview(bool newShowPreview)
{
    mShowPreview = newShowPreview;
}

QString MessageUrl::cleanText(const QString &str)
{
    QString newStr = str;
    newStr.remove(QLatin1Char('\n'));
    return newStr;
}

void MessageUrl::generateHtmlDescription()
{
    mHtmlDescription.clear();
    if (!mPageTitle.isEmpty()) {
        mHtmlDescription = u"[%1](%2)"_s.arg(MessageUrl::cleanText(mPageTitle), mUrl);
    }
    if (!mDescription.isEmpty()) {
        mHtmlDescription += u"\n%1"_s.arg(MessageUrl::cleanText(mDescription));
    }
    if (!mSiteName.isEmpty()) {
        mHtmlDescription += u"\n[%1](%2)"_s.arg(mSiteName, mSiteUrl);
    }
}

bool MessageUrl::hasHtmlDescription() const
{
    return !mHtmlDescription.isEmpty();
}

QString MessageUrl::htmlDescription() const
{
    return mHtmlDescription;
}

bool MessageUrl::hasPreviewUrl() const
{
    if (!mPageTitle.isEmpty() || !mDescription.isEmpty()) {
        return true;
    }
    if (!mImageUrl.isEmpty()) {
        return true;
    }
    if (hasPreviewContentType()) {
        return true;
    }
    return false;
}

QByteArray MessageUrl::urlId() const
{
    return mUrlId;
}

void MessageUrl::setUrlId(const QByteArray &newUrlId)
{
    mUrlId = newUrlId;
}

int MessageUrl::imageWidth() const
{
    return mImageWidth;
}

void MessageUrl::setImageWidth(int newImageWidth)
{
    mImageWidth = newImageWidth;
}

int MessageUrl::imageHeight() const
{
    return mImageHeight;
}

void MessageUrl::setImageHeight(int newImageHeight)
{
    mImageHeight = newImageHeight;
}

QString MessageUrl::siteName() const
{
    return mSiteName;
}

void MessageUrl::setSiteName(const QString &newSiteName)
{
    mSiteName = newSiteName;
}

QString MessageUrl::siteUrl() const
{
    return mSiteUrl;
}

void MessageUrl::setSiteUrl(const QString &newSiteUrl)
{
    mSiteUrl = newSiteUrl;
}

QString MessageUrl::authorUrl() const
{
    return mAuthorUrl;
}

void MessageUrl::setAuthorUrl(const QString &newAuthorUrl)
{
    mAuthorUrl = newAuthorUrl;
}

QString MessageUrl::authorName() const
{
    return mAuthorName;
}

void MessageUrl::setAuthorName(const QString &newAuthorName)
{
    mAuthorName = newAuthorName;
}

void MessageUrl::generateImageUrl()
{
    if (!mImageBuildUrl.isEmpty()) {
        return;
    }

    if (hasPreviewContentType()) {
        mImageBuildUrl = url();
    } else {
        if (mImageUrl.isEmpty()) {
            return;
        }
        const QUrl newUrl = QUrl(mImageUrl);
        if (!newUrl.isRelative()) {
            mImageBuildUrl = mImageUrl;
        } else {
            mImageBuildUrl = url();
            if (!url().endsWith(QLatin1Char('/')) && !mImageUrl.startsWith(QLatin1Char('/'))) {
                mImageBuildUrl += QLatin1Char('/');
            }
            mImageBuildUrl += mImageUrl;
#if 0
            QUrl u(url());
            u = u.adjusted(QUrl::RemoveFilename);
            u = u.resolved(QUrl(mImageUrl));
            mImageBuildUrl = u.path();
            qDebug() << " old url" << mImageBuildUrl << " u " << u;
#endif
        }
    }
}

MessageUrl::ContentType MessageUrl::contentType() const
{
    return mContentType;
}

void MessageUrl::setContentType(ContentType newContentType)
{
    mContentType = newContentType;
}

QString MessageUrl::buildImageUrl() const
{
    return mImageBuildUrl;
}

QString MessageUrl::imageUrl() const
{
    return mImageUrl;
}

void MessageUrl::setImageUrl(const QString &newImageUrl)
{
    mImageUrl = newImageUrl;
}

QString MessageUrl::contentTypeEnumToString(ContentType type)
{
    switch (type) {
    case ContentType::None:
        return {};
    case ContentType::Image:
        return QStringLiteral("image");
    case ContentType::ImageAnimated:
        return QStringLiteral("image_animated");
    case ContentType::Audio:
        return QStringLiteral("audio");
    case ContentType::Video:
        return QStringLiteral("video");
    }
    return {};
}

bool MessageUrl::hasPreviewContentType() const
{
    return mContentType != MessageUrl::ContentType::None;
}

MessageUrl::ContentType MessageUrl::stringToContentTypeEnum(const QString &str)
{
    if (str.isEmpty()) {
        return MessageUrl::ContentType::None;
    }
    if (str == "image"_L1) {
        return MessageUrl::ContentType::Image;
    }
    if (str == "audio"_L1) {
        return MessageUrl::ContentType::Audio;
    }
    if (str == "video"_L1) {
        return MessageUrl::ContentType::Video;
    }
    if (str == "image_animated"_L1) {
        return MessageUrl::ContentType::ImageAnimated;
    }
    return MessageUrl::ContentType::None;
}

MessageUrl::ContentType MessageUrl::parseHeaderContentType(const QString &typeHeader) const
{
    if (!typeHeader.isEmpty()) {
        const static QRegularExpression rimage(u"image/.*"_s);
        const static QRegularExpression raudio(u"audio/.*"_s);
        const static QRegularExpression rvideo(u"video/.*"_s);
        const static QRegularExpression rhtml(u"text/html.*"_s);
        if (typeHeader.contains("image/gif"_L1)) {
            return MessageUrl::ContentType::ImageAnimated;
        } else if (typeHeader.contains(rimage)) {
            return MessageUrl::ContentType::Image;
        } else if (typeHeader.contains(raudio)) {
            return MessageUrl::ContentType::Audio;
        } else if (typeHeader.contains(rvideo)) {
            return MessageUrl::ContentType::Video;
        } else if (typeHeader.contains(rhtml)) {
            return MessageUrl::ContentType::None;
        } else {
            qCDebug(RUQOLA_MESSAGE_URL_LOG) << "Invalid content type " << typeHeader << " this " << *this;
        }
    }
    return MessageUrl::ContentType::None;
}

void MessageUrl::parseUrl(const QJsonObject &url)
{
    const QJsonValue urlStr = url.value("url"_L1);
    if (!urlStr.isUndefined()) {
        setUrl(urlStr.toString());
    }
    // parse headers
    const QJsonObject headers = url.value("headers"_L1).toObject();
    const QString typeHeader = headers.value("contentType"_L1).toString();
    mContentType = parseHeaderContentType(typeHeader);

    const QJsonObject meta = url.value("meta"_L1).toObject();
    if (!meta.isEmpty()) {
        const auto titleElements = pageTitleElements();
        for (const QString &element : titleElements) {
            const QJsonValue pageTitleStr = meta.value(element);
            if (!pageTitleStr.isUndefined()) {
                setPageTitle(pageTitleStr.toString());
                break;
            }
        }
        const auto descElements = descriptionElements();
        for (const QString &element : descElements) {
            const QJsonValue descriptionStr = meta.value(element);
            if (!descriptionStr.isUndefined()) {
                setDescription(descriptionStr.toString());
                break;
            }
        }
        for (const QString &element : imageUrlElements()) {
            const QJsonValue imageUrlStr = meta.value(element);
            if (!imageUrlStr.isUndefined()) {
                setImageUrl(imageUrlStr.toString());
                break;
            }
        }
        const QJsonValue authorName = meta.value("oembedAuthorName"_L1);
        if (!authorName.isUndefined()) {
            setAuthorName(authorName.toString());
        }
        const QJsonValue authorUrl = meta.value("oembedAuthorUrl"_L1);
        if (!authorUrl.isUndefined()) {
            setAuthorUrl(authorUrl.toString());
        }
        for (const QString &element : siteUrlElements()) {
            const QJsonValue siteUrlStr = meta.value(element);
            if (!siteUrlStr.isUndefined()) {
                setSiteUrl(siteUrlStr.toString());
                break;
            }
        }
        for (const QString &element : siteNameElements()) {
            const QJsonValue siteNameStr = meta.value(element);
            if (!siteNameStr.isUndefined()) {
                setSiteName(siteNameStr.toString());
                break;
            }
        }
        for (const QString &element : imageHeightElements()) {
            const QJsonValue imageHeightStr = meta.value(element);
            if (!imageHeightStr.isUndefined()) {
                setImageHeight(imageHeightStr.toString().toInt());
                break;
            }
        }
        for (const QString &element : imageWidthElements()) {
            const QJsonValue imageWidthStr = meta.value(element);
            if (!imageWidthStr.isUndefined()) {
                setImageWidth(imageWidthStr.toString().toInt());
                break;
            }
        }
    }
    generateMessageUrlInfo();
    // qDebug() << " *this " << *this << " is empty " << isEmpty() << " url" << url;
    // Use apps/meteor/client/components/message/content/UrlPreviews.tsx
}

void MessageUrl::generateMessageUrlInfo()
{
    generateHtmlDescription();
    generateImageUrl();
}

QJsonObject MessageUrl::serialize(const MessageUrl &url)
{
    QJsonObject obj;
    if (!url.pageTitle().isEmpty()) {
        obj["pageTitle"_L1] = url.pageTitle();
    }
    obj["url"_L1] = url.url();
    if (!url.description().isEmpty()) {
        obj["description"_L1] = url.description();
    }
    if (!url.imageUrl().isEmpty()) {
        obj["imageUrl"_L1] = url.imageUrl();
    }
    if (!url.authorName().isEmpty()) {
        obj["authorName"_L1] = url.authorName();
    }
    if (!url.authorUrl().isEmpty()) {
        obj["authorUrl"_L1] = url.authorUrl();
    }
    if (!url.siteUrl().isEmpty()) {
        obj["siteUrl"_L1] = url.siteUrl();
    }
    if (!url.siteName().isEmpty()) {
        obj["siteName"_L1] = url.siteName();
    }
    if (url.imageHeight() > -1) {
        obj["imageHeight"_L1] = url.imageHeight();
    }
    if (url.imageWidth() > -1) {
        obj["imageWidth"_L1] = url.imageWidth();
    }
    if (url.contentType() != MessageUrl::ContentType::None) {
        obj["contentType"_L1] = contentTypeEnumToString(url.contentType());
    }
    if (!url.showPreview()) {
        obj["showPreview"_L1] = false;
    }
    return obj;
}

MessageUrl MessageUrl::deserialize(const QJsonObject &o)
{
    MessageUrl url;
    url.setPageTitle(o.value("pageTitle"_L1).toString());
    url.setUrl(o.value("url"_L1).toString());
    url.setDescription(o.value("description"_L1).toString());
    url.setImageUrl(o.value("imageUrl"_L1).toString());
    url.setAuthorName(o.value("authorName"_L1).toString());
    url.setAuthorUrl(o.value("authorUrl"_L1).toString());
    url.setSiteUrl(o.value("siteUrl"_L1).toString());
    url.setSiteName(o.value("siteName"_L1).toString());
    url.setImageHeight(o.value("imageHeight"_L1).toInt(-1));
    url.setImageWidth(o.value("imageWidth"_L1).toInt(-1));
    url.setContentType(stringToContentTypeEnum(o.value("contentType"_L1).toString()));
    url.setShowPreview(o.value("showPreview"_L1).toBool(true));
    url.generateMessageUrlInfo();
    return url;
}

QString MessageUrl::url() const
{
    return mUrl;
}

void MessageUrl::setUrl(const QString &url)
{
    mUrl = url;
}

QString MessageUrl::pageTitle() const
{
    return mPageTitle;
}

void MessageUrl::setPageTitle(const QString &pageTitle)
{
    mPageTitle = pageTitle;
}

bool MessageUrl::isEmpty() const
{
    return mUrl.isEmpty();
}

QString MessageUrl::description() const
{
    return mDescription;
}

void MessageUrl::setDescription(const QString &description)
{
    mDescription = description;
}

bool MessageUrl::operator==(const MessageUrl &other) const
{
    return (mUrl == other.url()) && (mPageTitle == other.pageTitle()) && (mDescription == other.description()) && (mImageUrl == other.imageUrl())
        && (mAuthorName == other.authorName()) && (mAuthorUrl == other.authorUrl()) && (mSiteUrl == other.siteUrl()) && (mSiteName == other.siteName())
        && (mImageHeight == other.imageHeight()) && (mImageWidth == other.imageWidth())
        && (mUrlId == other.urlId() && (mHtmlDescription == other.htmlDescription()) && (mImageBuildUrl == other.buildImageUrl()))
        && (mContentType == other.contentType());
}

QDebug operator<<(QDebug d, const MessageUrl &t)
{
    d.space() << "Url:" << t.url();
    d.space() << "Page Title:" << t.pageTitle();
    d.space() << "Description:" << t.description();
    d.space() << "ImageUrl:" << t.imageUrl();
    d.space() << "AuthorName:" << t.authorName();
    d.space() << "AuthorUrl:" << t.authorUrl();
    d.space() << "SiteUrl:" << t.siteUrl();
    d.space() << "SiteName:" << t.siteName();
    d.space() << "ImageHeight:" << t.imageHeight();
    d.space() << "ImageWidth:" << t.imageWidth();
    d.space() << "UrlId:" << t.urlId();
    d.space() << "htmlDescription:" << t.htmlDescription();
    d.space() << "buildImageUrl:" << t.buildImageUrl();
    d.space() << "contentType:" << t.contentType();
    return d;
}

#include "moc_messageurl.cpp"

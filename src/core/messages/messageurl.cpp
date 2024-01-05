/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurl.h"

#include <QJsonObject>

MessageUrl::MessageUrl() = default;

QStringList MessageUrl::pageTitleElements() const
{
    return {QStringLiteral("ogTitle"), QStringLiteral("twitterTitle"), QStringLiteral("title"), QStringLiteral("pageTitle"), QStringLiteral("oembedTitle")};
}

QStringList MessageUrl::descriptionElements() const
{
    return {QStringLiteral("ogDescription"), QStringLiteral("twitterDescription"), QStringLiteral("description")};
}

QStringList MessageUrl::imageUrlElements() const
{
    return {QStringLiteral("ogImage"),
            QStringLiteral("twitterImage"),
            QStringLiteral("msapplicationTileImage"),
            QStringLiteral("oembedThumbnailUrl"),
            QStringLiteral("oembedThumbnailUrl")};
}

QString MessageUrl::imageUrl() const
{
    return mImageUrl;
}

void MessageUrl::setImageUrl(const QString &newImageUrl)
{
    mImageUrl = newImageUrl;
}

void MessageUrl::parseUrl(const QJsonObject &url)
{
    const QJsonValue urlStr = url.value(QLatin1String("url"));
    if (!urlStr.isUndefined()) {
        setUrl(urlStr.toString());
    }
    const QJsonObject meta = url.value(QLatin1String("meta")).toObject();
    if (!meta.isEmpty()) {
        for (const QString &element : pageTitleElements()) {
            const QJsonValue pageTitleStr = meta.value(element);
            if (!pageTitleStr.isUndefined()) {
                setPageTitle(pageTitleStr.toString());
                break;
            }
        }
        for (const QString &element : descriptionElements()) {
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
    }

    // qDebug() << " *this " << *this << " is empty " << isEmpty();
    // Use apps/meteor/client/components/message/content/UrlPreviews.tsx
}

QJsonObject MessageUrl::serialize(const MessageUrl &url)
{
    QJsonObject obj;
    obj[QLatin1String("pageTitle")] = url.pageTitle();
    obj[QLatin1String("url")] = url.url();
    obj[QLatin1String("description")] = url.description();
    if (!url.imageUrl().isEmpty()) {
        obj[QLatin1String("imageUrl")] = url.imageUrl();
    }
    // TODO add more "ogTitle/ogDescription/ogUrl/ogImage/ogSiteName"
    return obj;
}

MessageUrl MessageUrl::deserialize(const QJsonObject &o)
{
    MessageUrl url;
    url.setPageTitle(o.value(QLatin1String("pageTitle")).toString());
    url.setUrl(o.value(QLatin1String("url")).toString());
    url.setDescription(o.value(QLatin1String("description")).toString());
    url.setImageUrl(o.value(QLatin1String("imageUrl")).toString());
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
    return (mUrl == other.url()) && (mPageTitle == other.pageTitle()) && (mDescription == other.description()) && (mImageUrl == other.imageUrl());
}

QDebug operator<<(QDebug d, const MessageUrl &t)
{
    d.space() << "Url:" << t.url();
    d.space() << "Page Title:" << t.pageTitle();
    d.space() << "Description:" << t.description();
    d.space() << "ImageURl:" << t.imageUrl();
    return d;
}

#include "moc_messageurl.cpp"

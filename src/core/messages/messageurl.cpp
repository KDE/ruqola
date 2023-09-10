/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurl.h"

#include <QJsonObject>

MessageUrl::MessageUrl() = default;

QJsonObject MessageUrl::serialize(const MessageUrl &url)
{
    QJsonObject obj;
    obj[QLatin1String("pagetitle")] = url.pageTitle();
    obj[QLatin1String("url")] = url.url();
    obj[QLatin1String("description")] = url.description();
    // TODO add more "ogTitle/ogDescription/ogUrl/ogImage/ogSiteName"
    return obj;
}

MessageUrl MessageUrl::deserialize(const QJsonObject &o)
{
    MessageUrl url;
    url.setPageTitle(o.value(QLatin1String("pagetitle")).toString());
    url.setUrl(o.value(QLatin1String("url")).toString());
    url.setDescription(o.value(QLatin1String("description")).toString());
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
    return (mUrl == other.url()) && (mPageTitle == other.pageTitle()) && (mDescription == other.description());
}

QDebug operator<<(QDebug d, const MessageUrl &t)
{
    d << "Url: " << t.url();
    d << "Page Title: " << t.pageTitle();
    d << "Description: " << t.description();
    return d;
}

#include "moc_messageurl.cpp"

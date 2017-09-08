/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messageurl.h"

#include <QJsonObject>

MessageUrl::MessageUrl()
{

}

QJsonObject MessageUrl::serialize(const MessageUrl &url)
{
    QJsonObject obj;
    obj[QStringLiteral("pagetitle")]  = url.pageTitle();
    obj[QStringLiteral("url")]  = url.url();
    return obj;
}

MessageUrl MessageUrl::fromJSon(const QJsonObject &o)
{
    MessageUrl url;
    url.setPageTitle(o.value(QStringLiteral("pagetitle")).toString());
    url.setUrl(o.value(QStringLiteral("url")).toString());
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

bool MessageUrl::operator==(const MessageUrl &other) const
{
    return (mUrl == other.url())
            && (mPageTitle == other.pageTitle());
}

QDebug operator <<(QDebug d, const MessageUrl &t)
{
    d << "Url: " << t.url();
    d << "Page Title: " << t.pageTitle();
    return d;
}

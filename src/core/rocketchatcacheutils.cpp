/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatcacheutils.h"

#include <QDebug>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
QString RocketChatCacheUtils::fileCachePath(const QUrl &url, ManagerDataPaths::PathType type, const QString &accountName, const QString &accountServerHost)
{
    QString cachePath = ManagerDataPaths::self()->path(type, accountName);
    // qDebug() << " cachePath" << cachePath;
    QString relativePathInCache = url.path();
    // qDebug() << " relativePathInCache" << relativePathInCache;
    const QString host = url.host();
    if (!host.isEmpty() && host != accountServerHost) {
        relativePathInCache.prepend(host + u'/');
    }
    if (type == ManagerDataPaths::PathType::PreviewUrl) {
        relativePathInCache = relativePathInCache.replace(u'/', u'_');
    }
    cachePath += u'/' + relativePathInCache;
    if (url.hasQuery()) {
        const QUrlQuery query(url);
        if (query.hasQueryItem(u"etag"_s)) {
            cachePath += query.queryItemValue(u"etag"_s);
        }
        if (query.hasQueryItem(u"file"_s)) {
            QString fileQuery = query.queryItemValue(u"file"_s);
            // qDebug() << " fileQuery "<< fileQuery << " url.query " << url.query();
            fileQuery = fileQuery.replace(u'/', u'_');
            cachePath += fileQuery;
        }
    }
    return cachePath;
}

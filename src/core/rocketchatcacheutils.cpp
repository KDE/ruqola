/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatcacheutils.h"

#include <QCryptographicHash>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;

namespace
{
// NAME_MAX is 255 bytes on ext4/btrfs/APFS. Keep a large margin: download jobs may
// append a suffix, and Windows caps the whole path at 260 characters.
constexpr qsizetype maxFileNameSize = 120;

// Some servers (e.g. cloudinary preview urls) generate very long path elements. As
// PreviewUrl flattens the whole url into a single file name we can easily exceed
// NAME_MAX, and creating the file then fails. Replace only the offending element by
// a hash of itself: the directory layout is kept, and two urls which used to share
// a cache entry still do.
[[nodiscard]] QString shortenedFileName(const QString &fileName)
{
    QString result = QString::fromLatin1(QCryptographicHash::hash(fileName.toUtf8(), QCryptographicHash::Sha256).toHex().left(32));
    // Keep a plausible extension: image loaders and mimetype detection rely on it.
    const qsizetype dotIndex = fileName.lastIndexOf(u'.');
    if (dotIndex > 0 && (fileName.size() - dotIndex) <= 8) {
        result += QStringView(fileName).mid(dotIndex);
    }
    return result;
}
}

QString RocketChatCacheUtils::fileCachePath(const QUrl &url, ManagerDataPaths::PathType type, const QString &accountName, const QString &accountServerHost)
{
    const QString cachePath = ManagerDataPaths::self()->path(type, accountName);
    // qDebug() << " cachePath" << cachePath;
    QString relativePathInCache = url.path();
    // qDebug() << " relativePathInCache" << relativePathInCache;
    const QString host = url.host();
    if (!host.isEmpty() && host != accountServerHost) {
        relativePathInCache.prepend(host + u'/');
    }
    if (type == ManagerDataPaths::PathType::PreviewUrl) {
        relativePathInCache.replace(u'/', u'_');
    }
    if (url.hasQuery()) {
        const QUrlQuery query(url);
        if (query.hasQueryItem(u"etag"_s)) {
            relativePathInCache += query.queryItemValue(u"etag"_s);
        }
        if (query.hasQueryItem(u"file"_s)) {
            QString fileQuery = query.queryItemValue(u"file"_s);
            // qDebug() << " fileQuery "<< fileQuery << " url.query " << url.query();
            fileQuery.replace(u'/', u'_');
            relativePathInCache += fileQuery;
        }
    }
    QStringList pathElements = relativePathInCache.split(u'/');
    for (QString &pathElement : pathElements) {
        if (pathElement.toUtf8().size() > maxFileNameSize) {
            pathElement = shortenedFileName(pathElement);
        }
    }
    return cachePath + u'/' + pathElements.join(u'/');
}

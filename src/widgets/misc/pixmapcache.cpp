/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pixmapcache.h"
#include "ruqolawidgets_debug.h"
#include <QFileInfo>

void PixmapCache::setMaxEntries(int maxEntries)
{
    mCachedImages.setMaxEntries(maxEntries);
}

QPixmap PixmapCache::pixmapForLocalFile(const QString &path, qreal devicePixelRatio)
{
    return scaledPixmapForLocalFile(path, -1, devicePixelRatio);
}

QPixmap PixmapCache::scaledPixmapForLocalFile(const QString &path, int maxSize, qreal devicePixelRatio)
{
    const qreal dpr = devicePixelRatio > 0 ? devicePixelRatio : 1.0;
    const QString key = path + u'\n' + QString::number(maxSize) + u'@' + QString::number(dpr);
    auto pixmap = findCachedPixmap(key);
    if (!pixmap.isNull()) {
        return pixmap;
    }
    pixmap = QPixmap(path);
    if (pixmap.isNull()) {
        if (QFileInfo(path).isFile()) { // When url needs access it will failed
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << path << " from cache";
        }
        return pixmap;
    }
    if (maxSize > 0) {
        const int deviceSize = qRound(maxSize * dpr);
        pixmap = pixmap.scaled(deviceSize, deviceSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    pixmap.setDevicePixelRatio(dpr);
    insertCachedPixmap(key, pixmap);
    return pixmap;
}

QPixmap PixmapCache::findCachedPixmap(const QString &path)
{
    auto it = mCachedImages.find(path);
    return it == mCachedImages.end() ? QPixmap() : it->value;
}

void PixmapCache::insertCachedPixmap(const QString &path, const QPixmap &pixmap)
{
    mCachedImages.insert(path, pixmap);
}

void PixmapCache::clear()
{
    mCachedImages.clear();
}

void PixmapCache::remove(const QString &path)
{
    mCachedImages.remove(path);
}

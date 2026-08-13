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
    auto pixmap = findCachedPixmap(path);

    if (pixmap.isNull()) {
        pixmap = QPixmap(path);
        if (pixmap.isNull()) {
            if (QFileInfo(path).isFile()) { // When url needs access it will failed
                qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << path << " from cache";
            }
            return pixmap;
        }
        if (devicePixelRatio > 0) {
            pixmap.setDevicePixelRatio(devicePixelRatio);
        }
        insertCachedPixmap(path, pixmap);
    } else if (devicePixelRatio > 0 && !qFuzzyCompare(pixmap.devicePixelRatioF(), devicePixelRatio)) {
        // The screen (or its scale factor) changed: pay for the detach once and re-cache the result.
        pixmap.setDevicePixelRatio(devicePixelRatio);
        remove(path);
        insertCachedPixmap(path, pixmap);
    }

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

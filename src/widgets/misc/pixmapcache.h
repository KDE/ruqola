/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"

#include "lrucache.h"
#include <QPixmap>

// QPixmapCache is too small for the big images in messages, let's have our own LRU cache
class LIBRUQOLAWIDGETS_TESTS_EXPORT PixmapCache
{
public:
    void setMaxEntries(int maxEntries);

    // devicePixelRatio: when > 0, the pixmap is *stored* with this ratio, so that callers in the
    // paint/hit-test paths don't have to call QPixmap::setDevicePixelRatio() on the cached pixmap
    // (which detaches, i.e. deep-copies the image data, on every single call).
    [[nodiscard]] QPixmap pixmapForLocalFile(const QString &path, qreal devicePixelRatio = -1);

    [[nodiscard]] QPixmap findCachedPixmap(const QString &path);
    void insertCachedPixmap(const QString &path, const QPixmap &pixmap);
    void clear();
    void remove(const QString &path);

private:
    friend class PixmapCacheTest;
    LRUCache<QString, QPixmap> mCachedImages;
};

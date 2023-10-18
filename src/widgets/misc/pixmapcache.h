/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"

#include <QPixmap>
#include <lrucache.h>

// QPixmapCache is too small for the big images in messages, let's have our own LRU cache
class LIBRUQOLAWIDGETS_TESTS_EXPORT PixmapCache
{
public:
    void setMaxEntries(int maxEntries);

    [[nodiscard]] QPixmap pixmapForLocalFile(const QString &path);

    [[nodiscard]] QPixmap findCachedPixmap(const QString &path);
    void insertCachedPixmap(const QString &path, const QPixmap &pixmap);
    void clear();
    void remove(const QString &path);

private:
    friend class PixmapCacheTest;
    LRUCache<QString, QPixmap> mCachedImages;
};

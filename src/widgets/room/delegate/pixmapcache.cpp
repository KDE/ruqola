/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "pixmapcache.h"
#include "ruqolawidgets_debug.h"

QPixmap PixmapCache::pixmapForLocalFile(const QString &path)
{
    QPixmap pixmap = findCachedPixmap(path);
    if (pixmap.isNull()) {
        if (pixmap.load(path)) {
            insertCachedPixmap(path, pixmap);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << path;
        }
    }
    return pixmap;
}

QPixmap PixmapCache::findCachedPixmap(const QString &link)
{
    auto matchesLink = [&](const CachedImage &cached) {
                           return cached.link == link;
                       };
    auto it = std::find_if(mCachedImages.begin(), mCachedImages.end(), matchesLink);
    if (it == mCachedImages.end()) {
        return QPixmap();
    }
    QPixmap result = it->pixmap; // grab pixmap before 'it' gets invalidated
    // Move it to the front
    if (it != mCachedImages.begin()) {
        const auto idx = std::distance(mCachedImages.begin(), it);
        mCachedImages.move(idx, 0);
    }
    return result;
}

void PixmapCache::insertCachedPixmap(const QString &link, const QPixmap &pixmap)
{
    mCachedImages.prepend(CachedImage{link, pixmap});
    static const int s_maxCacheSize = 5;
    if (mCachedImages.size() > s_maxCacheSize) {
        mCachedImages.resize(s_maxCacheSize);
    }
}

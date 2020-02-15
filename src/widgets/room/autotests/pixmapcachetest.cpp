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

#include "pixmapcachetest.h"
#include "room/delegate/pixmapcache.h"

#include <QStandardPaths>
#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(PixmapCacheTest)

PixmapCacheTest::PixmapCacheTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void PixmapCacheTest::shouldCacheLastFivePixmaps()
{
    PixmapCache cache;
    for (int i = 1; i < 7; ++i) {
        const QString link = QStringLiteral("link") + QString::number(i);
        const QPixmap pix(i * 10, i * 10);
        cache.insertCachedPixmap(link, pix);
        QCOMPARE(cache.findCachedPixmap(link).height(), i * 10);
        QCOMPARE(cache.findCachedPixmap(QStringLiteral("link1")).height(), 10); // we keep using that one
    }
    QCOMPARE(cache.findCachedPixmap(QStringLiteral("link4")).height(), 40);
    QVERIFY(cache.findCachedPixmap(QStringLiteral("link2")).isNull()); // oldest one got evicted
}

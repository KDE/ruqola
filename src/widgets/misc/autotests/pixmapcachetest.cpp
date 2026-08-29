/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pixmapcachetest.h"
#include "misc/pixmapcache.h"

#include <QFileInfo>
#include <QImage>
#include <QTest>

QTEST_MAIN(PixmapCacheTest)
using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr int squareEdge = 64;
constexpr int wideWidth = 80;
constexpr int wideHeight = 40;
constexpr int iconSize = 22;

[[nodiscard]] bool writeImage(const QString &path, int width, int height)
{
    QImage img(width, height, QImage::Format_ARGB32);
    img.fill(Qt::red);
    return img.save(path, "PNG");
}
}

PixmapCacheTest::PixmapCacheTest(QObject *parent)
    : QObject{parent}
{
}

QString PixmapCacheTest::squarePath() const
{
    return mTempDir.filePath(u"square.png"_s);
}

QString PixmapCacheTest::widePath() const
{
    return mTempDir.filePath(u"wide.png"_s);
}

void PixmapCacheTest::initTestCase()
{
    QVERIFY(mTempDir.isValid());
    QVERIFY(writeImage(squarePath(), squareEdge, squareEdge));
    QVERIFY(writeImage(widePath(), wideWidth, wideHeight));
}

void PixmapCacheTest::shouldReturnNullPixmapForMissingFile()
{
    PixmapCache cache;
    const QString missing = mTempDir.filePath(u"does-not-exist.png"_s);
    QVERIFY(!QFileInfo::exists(missing));
    QVERIFY(cache.pixmapForLocalFile(missing).isNull());
    QVERIFY(cache.scaledPixmapForLocalFile(missing, iconSize).isNull());
}

void PixmapCacheTest::shouldNotCacheMissingFile()
{
    PixmapCache cache;
    const QString missing = mTempDir.filePath(u"does-not-exist.png"_s);
    QVERIFY(cache.scaledPixmapForLocalFile(missing, iconSize).isNull());
    // A failed load must not poison the cache with a null entry.
    QCOMPARE(cache.mCachedImages.size(), std::size_t(0));
}

void PixmapCacheTest::shouldReturnUnscaledPixmap()
{
    PixmapCache cache;
    // maxSize == -1 means "native size": this is what the image attachment and url preview
    // helpers rely on, they scale the pixmap themselves against the available rect.
    const QPixmap pixmap = cache.pixmapForLocalFile(squarePath());
    QVERIFY(!pixmap.isNull());
    QCOMPARE(pixmap.size(), QSize(squareEdge, squareEdge));

    const QPixmap withDpr = cache.pixmapForLocalFile(widePath(), 2.0);
    QVERIFY(!withDpr.isNull());
    QCOMPARE(withDpr.size(), QSize(wideWidth, wideHeight));
    QCOMPARE(withDpr.devicePixelRatioF(), 2.0);
}

void PixmapCacheTest::shouldReuseCachedUnscaledPixmap()
{
    PixmapCache cache;
    const QPixmap first = cache.pixmapForLocalFile(squarePath());
    QVERIFY(!first.isNull());
    const QPixmap second = cache.pixmapForLocalFile(squarePath());
    // Same underlying data: the file must not be decoded again on every paint.
    QCOMPARE(second.cacheKey(), first.cacheKey());
    QCOMPARE(cache.mCachedImages.size(), std::size_t(1));
}

void PixmapCacheTest::shouldScaleToRequestedSize()
{
    PixmapCache cache;
    const QPixmap pixmap = cache.scaledPixmapForLocalFile(squarePath(), iconSize, 1.0);
    QVERIFY(!pixmap.isNull());
    QCOMPARE(pixmap.size(), QSize(iconSize, iconSize));
    QCOMPARE(pixmap.devicePixelRatioF(), 1.0);
}

void PixmapCacheTest::shouldScaleForDevicePixelRatio()
{
    PixmapCache cache;
    const QPixmap pixmap = cache.scaledPixmapForLocalFile(squarePath(), iconSize, 2.0);
    QVERIFY(!pixmap.isNull());
    // Scaled to device pixels...
    QCOMPARE(pixmap.size(), QSize(iconSize * 2, iconSize * 2));
    QCOMPARE(pixmap.devicePixelRatioF(), 2.0);
    // ...so that it still covers iconSize logical pixels, which is what layoutReactions()
    // reserved for it.
    QCOMPARE(pixmap.deviceIndependentSize(), QSizeF(iconSize, iconSize));
}

void PixmapCacheTest::shouldReuseCachedScaledPixmap()
{
    PixmapCache cache;
    const QPixmap first = cache.scaledPixmapForLocalFile(squarePath(), iconSize, 2.0);
    QVERIFY(!first.isNull());
    const QPixmap second = cache.scaledPixmapForLocalFile(squarePath(), iconSize, 2.0);
    // The whole point of the cache: no reload, and above all no SmoothTransformation rescale
    // on every paint event.
    QCOMPARE(second.cacheKey(), first.cacheKey());
    QCOMPARE(cache.mCachedImages.size(), std::size_t(1));
}

void PixmapCacheTest::shouldKeepAspectRatio()
{
    PixmapCache cache;
    const QPixmap pixmap = cache.scaledPixmapForLocalFile(widePath(), 20, 1.0);
    QVERIFY(!pixmap.isNull());
    QCOMPARE(pixmap.size(), QSize(20, 10));
}

void PixmapCacheTest::shouldNotShareEntryBetweenScaledAndUnscaled()
{
    PixmapCache cache;
    // Same path, two different requested sizes: the cache key must tell them apart, otherwise
    // whoever calls first decides the size everybody else gets.
    const QPixmap unscaled = cache.pixmapForLocalFile(squarePath());
    const QPixmap scaled = cache.scaledPixmapForLocalFile(squarePath(), iconSize, 1.0);
    QVERIFY(!unscaled.isNull());
    QVERIFY(!scaled.isNull());
    QCOMPARE(unscaled.size(), QSize(squareEdge, squareEdge));
    QCOMPARE(scaled.size(), QSize(iconSize, iconSize));
    QCOMPARE(cache.mCachedImages.size(), std::size_t(2));

    // ...and in the reverse order too.
    PixmapCache reversed;
    const QPixmap scaledFirst = reversed.scaledPixmapForLocalFile(squarePath(), iconSize, 1.0);
    const QPixmap unscaledSecond = reversed.pixmapForLocalFile(squarePath());
    QCOMPARE(scaledFirst.size(), QSize(iconSize, iconSize));
    QCOMPARE(unscaledSecond.size(), QSize(squareEdge, squareEdge));
}

void PixmapCacheTest::shouldNotShareEntryBetweenDevicePixelRatios()
{
    PixmapCache cache;
    const QPixmap onex = cache.scaledPixmapForLocalFile(squarePath(), iconSize, 1.0);
    const QPixmap twox = cache.scaledPixmapForLocalFile(squarePath(), iconSize, 2.0);
    QCOMPARE(onex.size(), QSize(iconSize, iconSize));
    QCOMPARE(twox.size(), QSize(iconSize * 2, iconSize * 2));
    QCOMPARE(cache.mCachedImages.size(), std::size_t(2));
}

void PixmapCacheTest::shouldRespectMaxEntries()
{
    PixmapCache cache;
    cache.setMaxEntries(1);
    QVERIFY(!cache.scaledPixmapForLocalFile(squarePath(), iconSize, 1.0).isNull());
    QVERIFY(!cache.scaledPixmapForLocalFile(widePath(), iconSize, 1.0).isNull());
    QCOMPARE(cache.mCachedImages.size(), std::size_t(1));
}

void PixmapCacheTest::shouldClearCache()
{
    PixmapCache cache;
    QVERIFY(!cache.scaledPixmapForLocalFile(squarePath(), iconSize, 1.0).isNull());
    QVERIFY(!cache.pixmapForLocalFile(widePath()).isNull());
    QCOMPARE(cache.mCachedImages.size(), std::size_t(2));
    cache.clear();
    QCOMPARE(cache.mCachedImages.size(), std::size_t(0));
}

#include "moc_pixmapcachetest.cpp"

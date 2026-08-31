/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatcachetest.h"
#include "rocketchatcacheutils.h"
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(RocketChatCacheTest)
using namespace Qt::Literals::StringLiterals;
RocketChatCacheTest::RocketChatCacheTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RocketChatCacheTest::shouldGenerateFileCachePath()
{
    QFETCH(QUrl, url);
    QFETCH(int, pathType);
    QFETCH(QString, result);
    const QString accountName(u"foo"_s);
    const QString accountServerHost(u"www.kde.org"_s);
    const QString cachePath = RocketChatCacheUtils::fileCachePath(url, static_cast<ManagerDataPaths::PathType>(pathType), accountName, accountServerHost);
    QCOMPARE(cachePath, result);
    // NAME_MAX is 255 bytes: a longer path element makes it impossible to create the file.
    const QStringList pathElements = cachePath.split(u'/');
    for (const QString &pathElement : pathElements) {
        QVERIFY(pathElement.toUtf8().size() <= 255);
    }
}

void RocketChatCacheTest::shouldGenerateFileCachePath_data()
{
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<int>("pathType");
    QTest::addColumn<QString>("result");
    const QString cacheLocationPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    QTest::addRow("empty") << QUrl() << 1 << cacheLocationPath + u"/foo/MainCache/"_s;

    QTest::addRow("test1") << QUrl(u"/file-upload/68f639d781d20ee79ad37dc0/YSKEtW.png"_s) << 1
                           << cacheLocationPath + u"/foo/MainCache//file-upload/68f639d781d20ee79ad37dc0/YSKEtW.png"_s;

    QTest::addRow("test2") << QUrl(u"https://www.kde.com/apps/files_sharing/preview/ooj3432?file=/2020-11-04T10_05_58.500044.jpg&x=4518&y=2542&a=true"_s) << 1
                           << cacheLocationPath + u"/foo/MainCache/www.kde.com//apps/files_sharing/preview/ooj3432_2020-11-04T10_05_58.500044.jpg"_s;

    // BUG: overlong path elements (here a cloudinary preview url flattened into a single
    // file name) are replaced by a hash, otherwise the file can't be created at all.
    QTest::addRow("too-long-preview-url") << QUrl(
        u"https://res.cloudinary.com/zenn/image/upload/s--bNE3XGWd--/c_fit%2Cg_north_west%2Cl_text:notosansjp-medium.otf_55:Memory%2520Usage%2520Benchmark%2520of%252015%2520Desktop%2520UI%2520Frameworks%2Cw_1010%2Cx_90%2Cy_100/g_south_west%2Cl_text:notosansjp-medium.otf_37:mizugeek%2Cx_203%2Cy_121/g_south_west%2Ch_90%2Cl_fetch:aHR0cHM6Ly9saDMuZ29vZ2xldXNlcmNvbnRlbnQuY29tL2EvQUNnOG9jSV9XbUNPZHcteDN5RjJGNDBiSTJEM2YyX1FLQjFpX1FHdkFTRFkzZ3FBZy1IbS0xOD1zOTYtYw==%2Cr_max%2Cw_90%2Cx_87%2Cy_95/v1627283836/default/og-base-w1200-v2.png?_a=BACMTiAE"_s)
                                          << 4 << cacheLocationPath + u"/foo/PreviewUrl/a5f8a12cc6ba9cd21d5b0fa0df29c7cd.png"_s;
}

#include "moc_rocketchatcachetest.cpp"

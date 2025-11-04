/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatcachetest.h"
#include "rocketchatcache.h"
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
    QCOMPARE(RocketChatCacheUtils::fileCachePath(url, static_cast<ManagerDataPaths::PathType>(pathType), accountName, accountServerHost), result);
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
}

#include "moc_rocketchatcachetest.cpp"

/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewurlcachemanagertest.h"
#include "previewurlcachemanager.h"
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>
QTEST_GUILESS_MAIN(PreviewUrlCacheManagerTest)

PreviewUrlCacheManagerTest::PreviewUrlCacheManagerTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void PreviewUrlCacheManagerTest::shouldHaveDefaultValues()
{
    PreviewUrlCacheManager w(nullptr);
    QCOMPARE(w.embedCacheExpirationDays(), -1);
    QVERIFY(w.cachePath().isEmpty());
}

void PreviewUrlCacheManagerTest::shouldTestRemoveOldFiles()
{
    QTemporaryDir accountFileTmp;
    const QString cachePath{accountFileTmp.path()};
    QFile file(cachePath + QStringLiteral("/foo1"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << " cccccccccccccccc";
    }
    qDebug() << " change date : " << file.setFileTime(QDateTime(QDate(2024, 1, 1), QTime(1, 1, 1)), QFileDevice::FileBirthTime);
    file.close();

    QDir dir(cachePath);
    const QFileInfoList infoLists = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    qDebug() << " infoLists " << infoLists.count() << infoLists;

    PreviewUrlCacheManager w(nullptr);
    w.setCachePath(cachePath);
    w.setEmbedCacheExpirationDays(2);

    QDir dir2(cachePath);
    const QFileInfoList infoLists2 = dir2.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    qDebug() << " infoLists2 " << infoLists2.count();
}

#include "moc_previewurlcachemanagertest.cpp"

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
    // Test 1: need to clean cache 1 file! 1 file to remove
    {
        QTemporaryDir accountFileTmp;
        const QString cachePath{accountFileTmp.path()};
        QFile file(cachePath + QStringLiteral("/foo1"));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << " Impossible to create file";
        }
        if (!file.setFileTime(QDateTime(QDate(2024, 1, 1), QTime(1, 1, 1)), QFileDevice::FileModificationTime)) {
            qWarning() << " Impossible to change modification date";
        }
        file.close();

        QDir dir(cachePath);
        const QFileInfoList infoLists = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        // qDebug() << "current infoLists " << infoLists.count() << infoLists;
        QCOMPARE(infoLists.count(), 1);

        PreviewUrlCacheManager w(nullptr);
        w.setCachePath(cachePath);
        w.setEmbedCacheExpirationDays(2);

        QDir dir2(cachePath);
        const QFileInfoList infoLists2 = dir2.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        // qDebug() << "after clean infoLists2 " << infoLists2.count();
        QVERIFY(infoLists2.isEmpty());
    }
    // Test 2: 10 files not necessary to clean them.
    {
        QTemporaryDir accountFileTmp;
        const QString cachePath{accountFileTmp.path()};
        QStringList firstList;
        for (int i = 0; i < 10; ++i) {
            QFile file(cachePath + QStringLiteral("/foo%1").arg(QString::number(i)));
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qWarning() << " Impossible to create file";
            }
            if (!file.setFileTime(QDateTime(QDate(2024, 1, i + 1), QTime(1, 1, 1)), QFileDevice::FileModificationTime)) {
                qWarning() << " Impossible to change modification date" << i;
            }
            firstList.append(QFileInfo(file).fileName());
            file.close();
        }

        QDir dir(cachePath);
        const QFileInfoList infoLists = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        // qDebug() << "current infoLists " << infoLists.count() << infoLists;
        QCOMPARE(infoLists.count(), 10);

        PreviewUrlCacheManager w(nullptr);
        w.setCachePath(cachePath);
        w.setEmbedCacheExpirationDays(5555); // 5555 days. It will not have pb :)

        QDir dir2(cachePath);
        const QFileInfoList infoLists2 = dir2.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
        QStringList lst;
        for (const QFileInfo &info : infoLists2) {
            lst.append(info.fileName());
        }
        // qDebug() << "after clean infoLists2 " << infoLists2.count();
        QVERIFY(!infoLists2.isEmpty());
        QCOMPARE(infoLists2.count(), 10);
        QCOMPARE(firstList, lst);
    }
}

#include "moc_previewurlcachemanagertest.cpp"

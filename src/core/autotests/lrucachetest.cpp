/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>
   SPDX-FileCopyrightText: 2020 Milian Wolff <mail@milianw.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "lrucachetest.h"

#include <QList>
#include <QStandardPaths>
#include <QTest>

#include "lrucache.h"
#include <algorithm>

QTEST_GUILESS_MAIN(LRUCacheTest)

LRUCacheTest::LRUCacheTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void LRUCacheTest::shouldCacheLastFiveEntries()
{
    auto makeString = [](const char *prefix, int i) -> QString {
        return QLatin1StringView(prefix) + QString::number(i);
    };

    using Cache = LRUCache<QString, QString>;
    Cache cache;
    cache.setMaxEntries(5);
    auto contents = [&cache]() -> QList<QString> {
        QList<QString> ret(cache.size());
        std::transform(cache.begin(), cache.end(), ret.begin(), [](const Cache::Entry &entry) {
            return entry.value;
        });
        return ret;
    };

    QCOMPARE(cache.size(), 0);
    QCOMPARE(cache.begin(), cache.end());
    QCOMPARE(cache.find(QString()), cache.end());

    QList<QString> expected;
    for (int i = 1; i < 7; ++i) {
        const auto expectedSizeBefore = std::min(i - 1, 5);
        const auto expectedSizeAfter = std::min(i, 5);
        QCOMPARE(cache.size(), expectedSizeBefore);
        const auto key = makeString("key", i);
        const auto value = makeString("value", i);
        QCOMPARE(cache.find(key), cache.end());
        cache.insert(key, value);
        QCOMPARE(cache.size(), expectedSizeAfter);
        QCOMPARE(std::distance(cache.begin(), cache.find(key)), 0);
        QCOMPARE(cache.find(key)->value, value);
        expected.prepend(value);
        if (expected.size() == 6) {
            expected.removeLast();
        }
        QCOMPARE(contents(), expected);
    }

    for (int i = 0; i < 10; ++i) {
        const auto key = makeString("key", i);
        const auto value = makeString("value", i);
        if (i <= 1 || i >= 7) {
            QCOMPARE(cache.find(key), cache.end());
        } else {
            QCOMPARE(std::distance(cache.begin(), cache.find(key)), 0);
            QCOMPARE(cache.find(key)->value, value);
            QCOMPARE(value, expected.last());
            expected.removeLast();
            expected.prepend(value);
        }
        QCOMPARE(contents(), expected);
    }

    // Looking up an entry moves it the front
    auto value = makeString("value", 4);
    QCOMPARE(cache.find(makeString("key", 4))->value, value);
    QVERIFY(expected.removeOne(value));
    expected.prepend(value);
    QCOMPARE(contents(), expected);

    // Remove an entry
    auto value3 = makeString("value", 3);
    QVERIFY(cache.remove(makeString("key", 3)));
    QVERIFY(expected.removeOne(value3));
    QCOMPARE(contents(), expected);

    cache.clear();
}

void LRUCacheTest::shouldWorkWithUniquePtr()
{
    static int deletions = 0;
    struct MyDocument {
        ~MyDocument()
        {
            ++deletions;
        }
    };
    LRUCache<int, std::unique_ptr<MyDocument>> documentCache;
    documentCache.setMaxEntries(32);
    documentCache.insert(42, std::make_unique<MyDocument>());
    QCOMPARE(documentCache.size(), 1);
    documentCache.clear();
    QCOMPARE(documentCache.size(), 0);
    QCOMPARE(deletions, 1);
}

#include "moc_lrucachetest.cpp"

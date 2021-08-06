/*
   Copyright (c) 2020 David Faure <faure@kde.org>
   Copyright (c) 2020 Milian Wolff <mail@milianw.de>

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

#include "lrucachetest.h"

#include <QStandardPaths>
#include <QTest>
#include <QVector>

#include <algorithm>
#include <lrucache.h>

QTEST_GUILESS_MAIN(LRUCacheTest)

LRUCacheTest::LRUCacheTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

namespace QTest
{
// Why does qtest.h have QList but not QVector support? Oh well, Qt6 unifies that.
template<typename T> inline bool qCompare(QVector<T> const &t1, QVector<T> const &t2, const char *actual, const char *expected, const char *file, int line)
{
    return qCompare(QList<T>(t1.begin(), t1.end()), QList<T>(t2.begin(), t2.end()), actual, expected, file, line);
}
}

void LRUCacheTest::shouldCacheLastFiveEntries()
{
    auto makeString = [](const char *prefix, int i) -> QString {
        return QLatin1String(prefix) + QString::number(i);
    };

    using Cache = LRUCache<QString, QString, 5>;
    Cache cache;
    auto contents = [&cache]() -> QVector<QString> {
        QVector<QString> ret(cache.size());
        std::transform(cache.begin(), cache.end(), ret.begin(), [](const Cache::Entry &entry) {
            return entry.value;
        });
        return ret;
    };

    QCOMPARE(cache.size(), 0);
    QCOMPARE(cache.begin(), cache.end());
    QCOMPARE(cache.find(QString()), cache.end());

    QVector<QString> expected;
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
    LRUCache<int, std::unique_ptr<MyDocument>, 32> documentCache;
    documentCache.insert(42, std::make_unique<MyDocument>());
    QCOMPARE(documentCache.size(), 1);
    documentCache.clear();
    QCOMPARE(documentCache.size(), 0);
    QCOMPARE(deletions, 1);
}

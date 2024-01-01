/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "discussionstest.h"
#include "discussions/discussion.h"
#include "discussions/discussions.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>

QTEST_GUILESS_MAIN(DiscussionsTest)

DiscussionsTest::DiscussionsTest(QObject *parent)
    : QObject(parent)
{
}

void DiscussionsTest::shouldHaveDefaultValues()
{
    Discussions w;
    QVERIFY(w.isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.total(), 0);
    QCOMPARE(w.discussionsCount(), 0);
}

void DiscussionsTest::shouldLoadDiscussions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("discussionsCount");
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("offset");

    QTest::addRow("empty") << QStringLiteral("discussions-empty") << 0 << 0 << 0;
    QTest::addRow("1-element") << QStringLiteral("discussions-1-element") << 1 << 1 << 0;
}

void DiscussionsTest::shouldLoadDiscussions()
{
    QFETCH(QString, name);
    QFETCH(int, discussionsCount);
    QFETCH(int, total);
    QFETCH(int, offset);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/discussions/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    Discussions m;
    m.parseDiscussions(obj);
    QCOMPARE(m.discussionsCount(), discussionsCount);
    QCOMPARE(m.total(), total);
    QCOMPARE(m.offset(), offset);
}

#include "moc_discussionstest.cpp"

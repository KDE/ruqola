/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "discussionstest.h"
#include "discussions.h"
#include "discussion.h"
#include "ruqola_autotest_helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
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

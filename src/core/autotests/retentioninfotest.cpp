/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "retentioninfotest.h"
#include "retentioninfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(RetentionInfoTest)
RetentionInfoTest::RetentionInfoTest(QObject *parent)
    : QObject(parent)
{
}

void RetentionInfoTest::shouldHaveDefaultValues()
{
    RetentionInfo info;
    QVERIFY(!info.enabled());
    QVERIFY(!info.overrideGlobal());
    QVERIFY(!info.excludePinned());
    QVERIFY(!info.filesOnly());
    QCOMPARE(info.maxAge(), -1);
}

void RetentionInfoTest::shouldLoadRetention_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<RetentionInfo>("retentionInfo");
    RetentionInfo info;
    info.setMaxAge(32);
    info.setEnabled(true);
    info.setExcludePinned(false);
    info.setFilesOnly(true);
    info.setOverrideGlobal(true);
    QTest::addRow("retention") << QStringLiteral("retention") << info;
}

void RetentionInfoTest::shouldLoadRetention()
{
    QFETCH(QString, name);
    QFETCH(RetentionInfo, retentionInfo);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/retention/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    RetentionInfo r;
    r.parseRetentionInfo(obj);
    const bool equalOwner = (r == retentionInfo);
    if (!equalOwner) {
        qDebug() << "ACTUAL " << r;
        qDebug() << "EXPECTED " << retentionInfo;
    }
    QVERIFY(equalOwner);
}

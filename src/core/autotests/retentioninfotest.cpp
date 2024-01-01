/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentioninfotest.h"
#include "retentioninfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>

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

#include "moc_retentioninfotest.cpp"

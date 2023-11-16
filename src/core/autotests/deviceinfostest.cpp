/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "deviceinfostest.h"
#include "managedevices/deviceinfos.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>

QTEST_GUILESS_MAIN(DeviceInfosTest)

DeviceInfosTest::DeviceInfosTest(QObject *parent)
    : QObject(parent)
{
}

void DeviceInfosTest::shouldHaveDefaultValues()
{
    DeviceInfos w;
    QVERIFY(w.isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.total(), 0);
    QCOMPARE(w.deviceInfosCount(), 0);
}

void DeviceInfosTest::shouldLoadDeviceInfos_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("deviceInfosCount");
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("offset");

    QTest::addRow("empty") << QStringLiteral("deviceinfos-empty") << 0 << 0 << 0;
    QTest::addRow("1-element") << QStringLiteral("deviceinfos-1-element") << 50 << 2 << 0;
}

void DeviceInfosTest::shouldLoadDeviceInfos()
{
    QFETCH(QString, name);
    QFETCH(int, deviceInfosCount);
    QFETCH(int, total);
    QFETCH(int, offset);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/deviceinfos/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    DeviceInfos m;
    m.parseDeviceInfos(obj);
    QCOMPARE(m.deviceInfosCount(), deviceInfosCount);
    QCOMPARE(m.total(), total);
    QCOMPARE(m.offset(), offset);
}

#include "moc_deviceinfostest.cpp"

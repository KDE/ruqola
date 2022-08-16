/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfotest.h"
#include "managedevices/deviceinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(DeviceInfoTest)
DeviceInfoTest::DeviceInfoTest(QObject *parent)
    : QObject{parent}
{
}

void DeviceInfoTest::shouldHaveDefaultValues()
{
    DeviceInfo w;
    QVERIFY(w.identifier().isEmpty());
    QVERIFY(w.host().isEmpty());
    QVERIFY(w.sessionId().isEmpty());
    QVERIFY(w.ip().isEmpty());
    QVERIFY(w.userId().isEmpty());
    QCOMPARE(w.loginAt(), -1);
    QVERIFY(w.os().isEmpty());
    QVERIFY(w.client().isEmpty());
}

void DeviceInfoTest::shouldDeviceInfo()
{
    QFETCH(QString, name);
    QFETCH(DeviceInfo, deviceInfo);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/deviceinfo/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    DeviceInfo result;
    result.parseDeviceInfo(obj);
    const bool equal = result == deviceInfo;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " deviceInfo " << deviceInfo;
    }
    QVERIFY(equal);
}

void DeviceInfoTest::shouldDeviceInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<DeviceInfo>("deviceInfo");
    QTest::addRow("deviceinfoempty") << QStringLiteral("deviceinfoempty") << DeviceInfo();
    {
    }
}

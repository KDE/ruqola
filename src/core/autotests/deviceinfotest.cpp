/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfotest.h"
#include "managedevices/deviceinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/deviceinfo/"_L1 + name + ".json"_L1;
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
    QTest::addRow("deviceinfoempty") << u"deviceinfoempty"_s << DeviceInfo();
    {
        DeviceInfo info;
        info.setHost(u"chat.kde.org"_s);
        info.setIdentifier("iy78NiKzd"_ba);
        info.setIp(u"179.4.8.1"_s);
        info.setSessionId("iKzd"_ba);
        info.setUserId("acidH"_ba);
        info.setLoginAt(1660405994979);
        QTest::addRow("deviceinfo1") << u"deviceinfo1"_s << info;
    }
    {
        DeviceInfo info;
        info.setHost(u"chat.kde.org"_s);
        info.setIdentifier("87pMW"_ba);
        info.setIp(u"1.7.6.11"_s);
        info.setSessionId("87pMW"_ba);
        info.setUserId("dacidH"_ba);
        info.setClient(u"Firefox"_s);
        info.setOs(u"Linux x86_64"_s);
        info.setLoginAt(1660404361954);
        QTest::addRow("deviceinfo2") << u"deviceinfo2"_s << info;
    }
}

#include "moc_deviceinfotest.cpp"

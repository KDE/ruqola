/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "deviceinfostest.h"
#include "managedevices/deviceinfos.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
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
    QTest::addColumn<QList<DeviceInfo>>("deviceInfo");

    QTest::addRow("empty") << QStringLiteral("deviceinfos-empty") << 0 << 0 << 0 << QList<DeviceInfo>();
    {
        QList<DeviceInfo> infos;
        {
            DeviceInfo info;
            info.setHost(QStringLiteral("kde.com"));
            info.setIdentifier(QByteArrayLiteral("LAS7ingGiy78NiKzd"));
            info.setIp(QStringLiteral("279.4.8.1"));
            info.setSessionId(QByteArrayLiteral("Giy78NiKzd"));
            info.setUserId(QByteArrayLiteral("uKK39zoewTkd"));
            info.setLoginAt(1660405994979);
            infos.append(info);
        }
        {
            DeviceInfo info;
            info.setHost(QStringLiteral("kde.com"));
            info.setIdentifier(QByteArrayLiteral("mxEY4NmYtise87pMW"));
            info.setIp(QStringLiteral("1.1.1.2"));
            info.setSessionId(QByteArrayLiteral("mxEse87pMW"));
            info.setUserId(QByteArrayLiteral("uKKkdacidH"));
            info.setLoginAt(1660404361954);
            info.setClient(QStringLiteral("Firefox"));
            info.setOs(QStringLiteral("Linux x86_64"));
            infos.append(info);
        }
        QTest::addRow("1-element") << QStringLiteral("deviceinfos-1-element") << 50 << 2 << 0 << infos;
    }
}

void DeviceInfosTest::shouldLoadDeviceInfos()
{
    QFETCH(QString, name);
    QFETCH(int, deviceInfosCount);
    QFETCH(int, total);
    QFETCH(int, offset);
    QFETCH(QList<DeviceInfo>, deviceInfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/deviceinfos/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    DeviceInfos m;
    m.parseDeviceInfos(obj);
    QCOMPARE(m.deviceInfosCount(), deviceInfosCount);
    QCOMPARE(m.total(), total);
    QCOMPARE(m.offset(), offset);
    QCOMPARE(m.deviceInfosList(), deviceInfo);
}

#include "moc_deviceinfostest.cpp"

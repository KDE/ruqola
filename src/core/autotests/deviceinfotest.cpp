/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfotest.h"
#include "managedevices/deviceinfo.h"
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
}

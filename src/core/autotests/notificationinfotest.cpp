/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationinfotest.h"
#include "notificationinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(NotificationInfoTest)

NotificationInfoTest::NotificationInfoTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationInfoTest::shouldHaveDefaultValues()
{
    NotificationInfo t;
    QVERIFY(t.accountName().isEmpty());
    QVERIFY(t.message().isEmpty());
    QVERIFY(t.title().isEmpty());
    QVERIFY(t.senderId().isEmpty());
    QVERIFY(t.senderName().isEmpty());
    QVERIFY(t.senderUserName().isEmpty());
    QVERIFY(t.roomName().isEmpty());
    QVERIFY(t.roomId().isEmpty());
    QVERIFY(t.channelType().isEmpty());
    QVERIFY(t.tmId().isEmpty());
    QVERIFY(t.pixmap().isNull());
}

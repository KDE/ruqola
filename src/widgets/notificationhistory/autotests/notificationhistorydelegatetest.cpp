/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydelegatetest.h"
#include "notificationhistory/notificationhistorydelegate.h"

#include <QTest>
QTEST_MAIN(NotificationHistoryDelegateTest)

NotificationHistoryDelegateTest::NotificationHistoryDelegateTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationHistoryDelegateTest::shouldCompareAccountInfo()
{
    {
        NotificationHistoryDelegate::RoomAccount info;
        info.accountName = QStringLiteral("bla");
        info.channelName = QStringLiteral("bli");

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = QStringLiteral("bla");
        info2.channelName = QStringLiteral("bli");
        QCOMPARE(info, info2);
    }

    {
        NotificationHistoryDelegate::RoomAccount info;
        info.accountName = QStringLiteral("zz");
        info.channelName = QStringLiteral("kk");

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = QStringLiteral("bla");
        info2.channelName = QStringLiteral("bli");
        QVERIFY(info != info2);
    }

    {
        NotificationHistoryDelegate::RoomAccount info;
        info.accountName = QStringLiteral("bla");
        info.channelName = QStringLiteral("kk");

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = QStringLiteral("bla");
        info2.channelName = QStringLiteral("bli");
        QVERIFY(info != info2);
    }
    {
        NotificationHistoryDelegate::RoomAccount info;
        info.accountName = QStringLiteral("ss");
        info.channelName = QStringLiteral("bli");

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = QStringLiteral("bla");
        info2.channelName = QStringLiteral("bli");
        QVERIFY(info != info2);
    }

    {
        NotificationHistoryDelegate::RoomAccount info;

        NotificationHistoryDelegate::RoomAccount info2;
        QCOMPARE(info, info2);
    }
}

#include "moc_notificationhistorydelegatetest.cpp"

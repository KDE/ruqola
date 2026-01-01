/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorydelegatetest.h"
using namespace Qt::Literals::StringLiterals;

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
        info.accountName = u"bla"_s;
        info.channelName = u"bli"_s;

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = u"bla"_s;
        info2.channelName = u"bli"_s;
        QCOMPARE(info, info2);
    }

    {
        NotificationHistoryDelegate::RoomAccount info;
        info.accountName = u"zz"_s;
        info.channelName = u"kk"_s;

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = u"bla"_s;
        info2.channelName = u"bli"_s;
        QVERIFY(info != info2);
    }

    {
        NotificationHistoryDelegate::RoomAccount info;
        info.accountName = u"bla"_s;
        info.channelName = u"kk"_s;

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = u"bla"_s;
        info2.channelName = u"bli"_s;
        QVERIFY(info != info2);
    }
    {
        NotificationHistoryDelegate::RoomAccount info;
        info.accountName = u"ss"_s;
        info.channelName = u"bli"_s;

        NotificationHistoryDelegate::RoomAccount info2;
        info2.accountName = u"bla"_s;
        info2.channelName = u"bli"_s;
        QVERIFY(info != info2);
    }

    {
        NotificationHistoryDelegate::RoomAccount info;

        NotificationHistoryDelegate::RoomAccount info2;
        QCOMPARE(info, info2);
    }
}

#include "moc_notificationhistorydelegatetest.cpp"

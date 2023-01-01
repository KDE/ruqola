/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountroomsettingstest.h"
#include "accountroomsettings.h"
#include <QTest>
QTEST_GUILESS_MAIN(AccountRoomSettingsTest)
AccountRoomSettingsTest::AccountRoomSettingsTest(QObject *parent)
    : QObject(parent)
{
}

void AccountRoomSettingsTest::shouldHaveDefaultValues()
{
    AccountRoomSettings s;
    QVERIFY(s.isEmpty());

    AccountRoomSettings::PendingTypedInfo info;
    QVERIFY(info.text.isEmpty());
    QVERIFY(info.messageIdBeingEdited.isEmpty());
    QVERIFY(info.threadMessageId.isEmpty());
    QCOMPARE(info.scrollbarPosition, -1);
    QVERIFY(!info.isValid());
}

void AccountRoomSettingsTest::shouldChangePendingTypedInfo()
{
    {
        AccountRoomSettings::PendingTypedInfo info;
        QVERIFY(!info.isValid());
        info.messageIdBeingEdited = QStringLiteral("foo");
        QVERIFY(info.isValid());
    }
    {
        AccountRoomSettings::PendingTypedInfo info;
        QVERIFY(!info.isValid());
        info.text = QStringLiteral("foo");
        QVERIFY(info.isValid());
    }
    {
        AccountRoomSettings::PendingTypedInfo info;
        QVERIFY(!info.isValid());
        info.scrollbarPosition = 2;
        QVERIFY(info.isValid());
    }
}

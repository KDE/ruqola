/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountroomsettingstest.h"
#include "accountroomsettings.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(AccountRoomSettingsTest)
AccountRoomSettingsTest::AccountRoomSettingsTest(QObject *parent)
    : QObject(parent)
{
}

void AccountRoomSettingsTest::shouldHaveDefaultValues()
{
    AccountRoomSettings s(nullptr);
    QVERIFY(s.isEmpty());

    const AccountRoomSettings::PendingTypedInfo info;
    QVERIFY(info.text.isEmpty());
    QVERIFY(info.messageIdBeingEdited.isEmpty());
    QVERIFY(info.threadMessageId.isEmpty());
    QCOMPARE(info.scrollbarPosition, -1);
    QVERIFY(!info.isValid());
    QVERIFY(!info.hasPendingMessageTyped());
}

void AccountRoomSettingsTest::shouldChangePendingTypedInfo()
{
    {
        AccountRoomSettings::PendingTypedInfo info;
        QVERIFY(!info.isValid());
        info.messageIdBeingEdited = "foo"_ba;
        QVERIFY(info.isValid());
        QVERIFY(!info.hasPendingMessageTyped());
    }
    {
        AccountRoomSettings::PendingTypedInfo info;
        QVERIFY(!info.isValid());
        info.text = u"foo"_s;
        QVERIFY(info.isValid());
        QVERIFY(info.hasPendingMessageTyped());
    }
    {
        AccountRoomSettings::PendingTypedInfo info;
        QVERIFY(!info.isValid());
        info.scrollbarPosition = 2;
        QVERIFY(info.isValid());
        QVERIFY(!info.hasPendingMessageTyped());
    }
}

void AccountRoomSettingsTest::shouldSerializeDeserialize()
{
    {
        AccountRoomSettings::PendingTypedInfo info;
        info.messageIdBeingEdited = "foo"_ba;
        info.text = u"bla"_s;

        QCOMPARE(AccountRoomSettings::PendingTypedInfo::deserialize(AccountRoomSettings::PendingTypedInfo::serialize(info)), info);
    }
    {
        AccountRoomSettings::PendingTypedInfo info;
        info.messageIdBeingEdited = "foo"_ba;
        info.text = u"bla"_s;
        info.scrollbarPosition = 90;

        QCOMPARE(AccountRoomSettings::PendingTypedInfo::deserialize(AccountRoomSettings::PendingTypedInfo::serialize(info)), info);
    }
    {
        AccountRoomSettings::PendingTypedInfo info;
        info.messageIdBeingEdited = "foo"_ba;
        info.text = u"bla"_s;
        info.scrollbarPosition = 90;
        info.threadMessageId = "you"_ba;
        info.quotePermalink = u"ddd"_s;
        info.quoteText = u"kde5"_s;

        QCOMPARE(AccountRoomSettings::PendingTypedInfo::deserialize(AccountRoomSettings::PendingTypedInfo::serialize(info)), info);
    }
}

#include "moc_accountroomsettingstest.cpp"

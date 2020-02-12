/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

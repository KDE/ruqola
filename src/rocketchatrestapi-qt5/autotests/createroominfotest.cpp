/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createroominfotest.h"
#include "createroominfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(CreateRoomInfoTest)

CreateRoomInfoTest::CreateRoomInfoTest(QObject *parent)
    : QObject(parent)
{
}

void CreateRoomInfoTest::shouldHaveDefaultValues()
{
    RocketChatRestApi::CreateRoomInfo info;
    QVERIFY(info.members.isEmpty());
    QVERIFY(info.name.isEmpty());
    QVERIFY(info.description.isEmpty());
    QVERIFY(info.teamId.isEmpty());
    QVERIFY(!info.readOnly);
    QVERIFY(!info.broadcast);
    QVERIFY(!info.encrypted);
    QVERIFY(!info.isValid());
}

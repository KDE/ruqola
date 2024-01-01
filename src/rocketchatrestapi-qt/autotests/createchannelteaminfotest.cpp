/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createchannelteaminfotest.h"
#include "createchannelteaminfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(CreateChannelTeamInfoTest)

CreateChannelTeamInfoTest::CreateChannelTeamInfoTest(QObject *parent)
    : QObject(parent)
{
}

void CreateChannelTeamInfoTest::shouldHaveDefaultValues()
{
    RocketChatRestApi::CreateChannelTeamInfo info;
    QVERIFY(info.members.isEmpty());
    QVERIFY(info.name.isEmpty());
    QVERIFY(info.description.isEmpty());
    QVERIFY(info.teamId.isEmpty());
    QVERIFY(!info.readOnly);
    QVERIFY(!info.broadcast);
    QVERIFY(!info.encrypted);
    QVERIFY(!info.isValid());
}

#include "moc_createchannelteaminfotest.cpp"

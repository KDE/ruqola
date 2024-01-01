/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomutiltest.h"
#include "room/roomutil.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomUtilTest)

RoomUtilTest::RoomUtilTest(QObject *parent)
    : QObject(parent)
{
}

void RoomUtilTest::shouldGeneratePermalink()
{
    QFETCH(QString, messageId);
    QFETCH(QString, roomId);
    QFETCH(Room::RoomType, channelType);
    QFETCH(QString, generatePermalink);

    QCOMPARE(RoomUtil::generatePermalink(messageId, roomId, channelType), generatePermalink);
}

void RoomUtilTest::shouldGeneratePermalink_data()
{
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<QString>("roomId");
    QTest::addColumn<Room::RoomType>("channelType");
    QTest::addColumn<QString>("generatePermalink");

    QTest::addRow("channels") << QStringLiteral("msId") << QStringLiteral("roomId") << Room::RoomType::Channel << QStringLiteral("channel/roomId?msg=msId");
    QTest::addRow("direct") << QStringLiteral("msId") << QStringLiteral("roomId") << Room::RoomType::Direct << QStringLiteral("direct/roomId?msg=msId");
    QTest::addRow("group") << QStringLiteral("msId") << QStringLiteral("roomId") << Room::RoomType::Private << QStringLiteral("group/roomId?msg=msId");
}

#include "moc_roomutiltest.cpp"

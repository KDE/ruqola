/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomutiltest.h"
using namespace Qt::Literals::StringLiterals;

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

    QTest::addRow("channels") << u"msId"_s << u"roomId"_s << Room::RoomType::Channel << QStringLiteral("channel/roomId?msg=msId");
    QTest::addRow("direct") << u"msId"_s << u"roomId"_s << Room::RoomType::Direct << QStringLiteral("direct/roomId?msg=msId");
    QTest::addRow("group") << u"msId"_s << u"roomId"_s << Room::RoomType::Private << QStringLiteral("group/roomId?msg=msId");
}

#include "moc_roomutiltest.cpp"

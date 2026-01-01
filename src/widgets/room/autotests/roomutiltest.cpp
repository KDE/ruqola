/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

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

    QTest::addRow("channels") << u"msId"_s << u"roomId"_s << Room::RoomType::Channel << u"channel/roomId?msg=msId"_s;
    QTest::addRow("direct") << u"msId"_s << u"roomId"_s << Room::RoomType::Direct << u"direct/roomId?msg=msId"_s;
    QTest::addRow("group") << u"msId"_s << u"roomId"_s << Room::RoomType::Private << u"group/roomId?msg=msId"_s;
}

#include "moc_roomutiltest.cpp"

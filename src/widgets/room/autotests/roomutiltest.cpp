/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

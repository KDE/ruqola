/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "roomtest.h"
#include "room.h"
#include <QJsonDocument>
#include <QTest>

QTEST_MAIN(RoomTest)

RoomTest::RoomTest(QObject *parent)
    : QObject(parent)
{

}

void RoomTest::shouldSerialized()
{
    Room input;
    input.setId(QStringLiteral("foo"));
    input.setChannelType(QStringLiteral("p"));
    input.setName(QStringLiteral("d"));
    input.setAnnouncement(QStringLiteral("AA"));
    input.setUserName(QStringLiteral("pp"));
    input.setUserId(QStringLiteral("sdfsdfs"));
    input.setTopic(QStringLiteral("topic"));
    input.setMutedUsers(QStringLiteral("mutedUsers"));
    input.setJitsiTimeout(55);
    input.setReadOnly(true);
    input.setUnread(66);
    input.setSelected(true);
    input.setFavorite(true);
    input.setAlert(true);
    input.setOpen(true);
    const QByteArray ba = Room::serialize(&input);
    Room *output = Room::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
    QVERIFY(input.isEqual(*output));
}

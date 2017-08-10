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


#include "roommodeltest.h"
#include "roommodel.h"
#include "room.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

QTEST_MAIN(RoomModelTest)

RoomModelTest::RoomModelTest(QObject *parent)
    : QObject(parent)
{

}

void RoomModelTest::shouldSerialized()
{
    Room input;
    input.id = QStringLiteral("foo");
    input.type = QStringLiteral("p");
    input.name = QStringLiteral("d");
    input.mAnnouncement = QStringLiteral("AA");
    input.userName = QStringLiteral("pp");
    input.userID = QStringLiteral("sdfsdfs");
    input.topic = QStringLiteral("topic");
    input.mutedUsers = QStringLiteral("mutedUsers");
    input.jitsiTimeout = 55;
    input.ro = true;
    input.unread = 66;
    input.selected = true;
    input.favorite = true;
    input.alert = true;
    input.open = true;
    const QByteArray ba = RoomModel::serialize(input);
    Room output = RoomModel::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
    QVERIFY(input.isEqual(output));
}

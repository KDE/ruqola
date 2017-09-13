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

#include "messagemodeltest.h"
#include "messagemodel.h"
#include <QJsonDocument>
#include <QTest>

QTEST_MAIN(MessageModelTest)

MessageModelTest::MessageModelTest(QObject *parent)
    : QObject(parent)
{
}

void MessageModelTest::shouldHaveDefaultValue()
{
    MessageModel w;
    QCOMPARE(w.rowCount(), 0);
}

void MessageModelTest::shouldAddMessage()
{
    MessageModel w;
    Message input;

    input.setMessageId(QStringLiteral("ff"));
    input.setRoomId(QStringLiteral("room2"));
    input.setText(QStringLiteral("message1"));
    input.setTimeStamp(42);
    input.setUsername(QStringLiteral("user1"));
    input.setUserId(QStringLiteral("userid1"));
    input.setUpdatedAt(45);
    input.setEditedAt(89);
    input.setEditedByUsername(QStringLiteral("editeduser1"));
    input.setEditedByUserId(QStringLiteral("editedbyid1"));
    input.setAlias(QStringLiteral("ali"));
    input.setAvatar(QStringLiteral("avatar1"));
    input.setSystemMessageType(QStringLiteral("type"));
    input.setGroupable(true);
    input.setParseUrls(true);
    input.setMessageType(Message::MessageType::Audio);
    w.addMessage(input);

    QCOMPARE(w.rowCount(), 1);
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 1);

    input.setMessageId(QStringLiteral("ff2"));
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 2);
}

//TODO add remove message test

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

void MessageModelTest::shouldSerializeData()
{
    Message input;

    input.messageID = QStringLiteral("ff");
    input.roomID = QStringLiteral("room1");
    input.message = QStringLiteral("message1");
    input.timestamp = 42;
    input.username = QStringLiteral("user1");
    input.userID = QStringLiteral("userid1");
    input.updatedAt = 45;
    input.editedAt = 89;
    input.editedByUsername = QStringLiteral("editeduser1");
    input.editedByUserID = QStringLiteral("editedbyid1");
    input.url = QStringLiteral("url1");
    input.meta = QStringLiteral("meta1");
    input.headers = QStringLiteral("headers111");
    input.parsedUrl = QStringLiteral("parseurl1");
    input.imageUrl = QStringLiteral("imageurl1");
    input.color = QStringLiteral("color1");
    input.alias = QStringLiteral("ali");
    input.avatar = QStringLiteral("avatar1");
    input.systemMessageType = QStringLiteral("type");
    input.groupable = true;
    input.parseUrls = true;

    input.systemMessage = true;
    const QByteArray ba = MessageModel::serialize(input);
    Message output = MessageModel::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
    QVERIFY(input.isEqual(output));
}

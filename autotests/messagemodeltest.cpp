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

    input.mMessageId = QStringLiteral("ff");
    input.mRoomId = QStringLiteral("room1");
    input.mText = QStringLiteral("message1");
    input.mTimeStamp = 42;
    input.mUsername = QStringLiteral("user1");
    input.mUserId = QStringLiteral("userid1");
    input.mUpdatedAt = 45;
    input.mEditedAt = 89;
    input.mEditedByUsername = QStringLiteral("editeduser1");
    input.mEditedByUserId = QStringLiteral("editedbyid1");
    input.mUrl = QStringLiteral("url1");
    input.mMeta = QStringLiteral("meta1");
    input.mHeaders = QStringLiteral("headers111");
    input.mParsedUrl = QStringLiteral("parseurl1");
    input.mImageUrl = QStringLiteral("imageurl1");
    input.mColor = QStringLiteral("color1");
    input.mAlias = QStringLiteral("ali");
    input.mAvatar = QStringLiteral("avatar1");
    input.mSystemMessageType = QStringLiteral("type");
    input.mGroupable = true;
    input.mParseUrls = true;

    input.mSystemMessage = true;
    const QByteArray ba = MessageModel::serialize(input);
    Message output = MessageModel::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
    QVERIFY(input.isEqual(output));
}

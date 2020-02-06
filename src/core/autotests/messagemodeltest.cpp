/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
#include "test_model_helpers.h"
#include "model/messagemodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(MessageModelTest)

MessageModelTest::MessageModelTest(QObject *parent)
    : QObject(parent)
{
}

void MessageModelTest::shouldHaveDefaultValue()
{
    //TODO add roqulaaccount
    MessageModel w;
    QCOMPARE(w.rowCount(), 0);

    QHash<int, QByteArray> roles;
    roles[MessageModel::OriginalMessage] = QByteArrayLiteral("originalMessage");
    roles[MessageModel::MessageConvertedText] = QByteArrayLiteral("messageConverted");
    roles[MessageModel::Username] = QByteArrayLiteral("username");
    roles[MessageModel::Timestamp] = QByteArrayLiteral("timestamp");
    roles[MessageModel::UserId] = QByteArrayLiteral("userID");
    roles[MessageModel::SystemMessageType] = QByteArrayLiteral("type");
    roles[MessageModel::MessageId] = QByteArrayLiteral("messageID");
    roles[MessageModel::RoomId] = QByteArrayLiteral("roomID");
    roles[MessageModel::UpdatedAt] = QByteArrayLiteral("updatedAt");
    roles[MessageModel::EditedAt] = QByteArrayLiteral("editedAt");
    roles[MessageModel::EditedByUserName] = QByteArrayLiteral("editedByUsername");
    roles[MessageModel::EditedByUserId] = QByteArrayLiteral("editedByUserID");
    roles[MessageModel::Alias] = QByteArrayLiteral("alias");
    roles[MessageModel::Avatar] = QByteArrayLiteral("avatar");
    roles[MessageModel::Groupable] = QByteArrayLiteral("groupable");
    roles[MessageModel::MessageType] = QByteArrayLiteral("messagetype");
    roles[MessageModel::Attachments] = QByteArrayLiteral("attachments");
    roles[MessageModel::Urls] = QByteArrayLiteral("urls");
    roles[MessageModel::Date] = QByteArrayLiteral("date");
    roles[MessageModel::CanEditMessage] = QByteArrayLiteral("canEditMessage");
    roles[MessageModel::Starred] = QByteArrayLiteral("starred");
    roles[MessageModel::UsernameUrl] = QByteArrayLiteral("usernameurl");
    roles[MessageModel::Roles] = QByteArrayLiteral("roles");
    roles[MessageModel::Reactions] = QByteArrayLiteral("reactions");
    roles[MessageModel::Ignored] = QByteArrayLiteral("userIsIgnored");
    roles[MessageModel::Pinned] = QByteArrayLiteral("pinned");

    roles[MessageModel::DiscussionCount] = QByteArrayLiteral("discussionCount");
    roles[MessageModel::DiscussionRoomId] = QByteArrayLiteral("discussionRoomId");
    roles[MessageModel::DiscussionLastMessage] = QByteArrayLiteral("discussionLastMessage");
    roles[MessageModel::ThreadCount] = QByteArrayLiteral("threadCount");
    roles[MessageModel::ThreadLastMessage] = QByteArrayLiteral("threadLastMessage");
    roles[MessageModel::ThreadMessageId] = QByteArrayLiteral("threadMessageId");
    roles[MessageModel::ThreadMessagePreview] = QByteArrayLiteral("threadMessagePreview");
    roles[MessageModel::ShowTranslatedMessage] = QByteArrayLiteral("showTranslatedMessage");
    roles[MessageModel::DisplayAttachment] = QByteArrayLiteral("displayAttachment");
    QCOMPARE(w.roleNames(), roles);
}

static void fillTestMessage(Message &input)
{
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
}

void MessageModelTest::shouldAddMessage()
{
    MessageModel w;
    Message input;
    fillTestMessage(input);
    input.setMessageId(QStringLiteral("ff"));
    w.addMessage(input);

    QCOMPARE(w.rowCount(), 1);
    //Don't create duplicates
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 1);

    //Add other messageId
    input.setMessageId(QStringLiteral("ff2"));
    input.setTimeStamp(43);
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 2);

    input.setMessageId(QStringLiteral("ff3"));
    input.setTimeStamp(44);
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 3);

    input.setMessageId(QStringLiteral("ff4"));
    input.setTimeStamp(45);
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 4);

    input.setMessageId(QStringLiteral("ff2"));
    input.setTimeStamp(43);
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 4);
}

void MessageModelTest::shouldRemoveMessage()
{
    MessageModel w;
    Message input;
    fillTestMessage(input);

    QSignalSpy rowInsertedSpy(&w, &MessageModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &MessageModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &MessageModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &MessageModel::rowsAboutToBeRemoved);

    const QString messageId = QStringLiteral("ff");
    input.setMessageId(messageId);
    w.addMessage(input);

    QCOMPARE(w.rowCount(), 1);
    //Remove existing message

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,0"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,0"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.deleteMessage(messageId);

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,0"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,0"));

    QCOMPARE(w.rowCount(), 0);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.addMessage(input);
    QCOMPARE(w.rowCount(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,0"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,0"));
}

void MessageModelTest::shouldRemoveNotExistingMessage()
{
    MessageModel w;
    Message input;
    fillTestMessage(input);
    const QString messageId = QStringLiteral("ff");
    input.setMessageId(messageId);
    w.addMessage(input);

    QCOMPARE(w.rowCount(), 1);
    //Remove non-existing message
    w.deleteMessage(QStringLiteral("Bla"));
    QCOMPARE(w.rowCount(), 1);

    input.setMessageId(QStringLiteral("ff3"));
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 2);

    input.setMessageId(QStringLiteral("ff4"));
    w.addMessage(input);
    QCOMPARE(w.rowCount(), 3);

    //Remove 3th element
    w.deleteMessage(QStringLiteral("ff3"));
    QCOMPARE(w.rowCount(), 2);
}

void MessageModelTest::shouldDetectDateChange()
{
    MessageModel model;
    Message first;
    first.setMessageId(QStringLiteral("first"));
    first.setTimeStamp(QDateTime(QDate(2019, 6, 7), QTime(23, 50, 50)).toMSecsSinceEpoch());
    model.addMessage(first);
    QVERIFY(model.index(0, 0).data(MessageModel::DateDiffersFromPrevious).toBool()); // first message

    Message second;
    second.setMessageId(QStringLiteral("second"));
    second.setTimeStamp(QDateTime(QDate(2019, 6, 8), QTime(1, 2, 3)).toMSecsSinceEpoch());
    model.addMessage(second);
    QCOMPARE(model.rowCount(), 2);
    QVERIFY(model.index(1, 0).data(MessageModel::DateDiffersFromPrevious).toBool()); // next day

    Message third;
    third.setTimeStamp(QDateTime(QDate(2019, 6, 8), QTime(1, 4, 3)).toMSecsSinceEpoch());
    third.setMessageId(QStringLiteral("third"));
    model.addMessage(third);
    QCOMPARE(model.rowCount(), 3);
    QVERIFY(!model.index(2, 0).data(MessageModel::DateDiffersFromPrevious).toBool()); // same day
}

static QByteArrayList extractMessageIds(MessageModel &m)
{
    QByteArrayList ret;
    const int count = m.rowCount();
    ret.reserve(count);
    for (int row = 0; row < count; ++row) {
        ret << m.index(row, 0).data(MessageModel::MessageId).toString().toLatin1();
    }
    return ret;
}

void MessageModelTest::shouldAddMessages()
{
    MessageModel model;
    Message input;
    fillTestMessage(input);
    QVector<Message> messages;
    auto makeMessage = [&](const char *id, qint64 timestamp) {
        input.setMessageId(QString::fromLatin1(id));
        input.setTimeStamp(timestamp);
        return input;
    };
    messages << makeMessage("msgA", 8);
    messages << makeMessage("msgB", 4);
    messages << makeMessage("msgC", 6);
    messages << makeMessage("msgD", 2);
    model.addMessages(messages);
    QCOMPARE(model.rowCount(), 4);
    QCOMPARE(extractMessageIds(model), QByteArrayList() << "msgD" << "msgB" << "msgC" << "msgA");

    messages.clear();
    messages << makeMessage("msgE", 1);
    messages << makeMessage("msgF", 3);
    messages << makeMessage("msgG", 9);
    messages << makeMessage("msgH", 5);
    input.setText(QStringLiteral("modified"));
    messages << makeMessage("msgA", 8); // update
    model.addMessages(messages);
    QCOMPARE(model.rowCount(), 8);
    QCOMPARE(extractMessageIds(model), QByteArrayList() << "msgE" << "msgD" << "msgF" << "msgB" << "msgH" << "msgC" << "msgA" << "msgG");
    QCOMPARE(model.index(6, 0).data(MessageModel::OriginalMessage).toString(), QStringLiteral("modified"));
}

void MessageModelTest::shouldUpdateFirstMessage()
{
    MessageModel model;
    Message input;
    fillTestMessage(input);
    model.addMessages( { input } );
    QCOMPARE(model.rowCount(), 1);
    input.setText(QStringLiteral("modified"));
    model.addMessages( { input } );
    QCOMPARE(model.index(0, 0).data(MessageModel::OriginalMessage).toString(), QStringLiteral("modified"));
}

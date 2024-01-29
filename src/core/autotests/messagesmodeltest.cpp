/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagesmodeltest.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "test_model_helpers.h"
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(MessagesModelTest)

MessagesModelTest::MessagesModelTest(QObject *parent)
    : QObject(parent)
{
}

void MessagesModelTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QVERIFY(rcAccount);
    rcAccount->settings()->setUserId(QStringLiteral("userid1"));
}

void MessagesModelTest::shouldHaveDefaultValue()
{
    MessagesModel w(QStringLiteral("roomId"), Ruqola::self()->rocketChatAccount());
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.searchText().isEmpty());
}

static void fillTestMessage(Message &input)
{
    input.setRoomId(QStringLiteral("room2"));
    input.setText(QStringLiteral("message1"));
    input.setTimeStamp(QDateTime::currentMSecsSinceEpoch());
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
    input.setMessageType(Message::MessageType::NormalText);
}

void MessagesModelTest::shouldAddMessage()
{
    MessagesModel w;
    Message input;
    fillTestMessage(input);
    input.setMessageId(QStringLiteral("ff"));
    w.addMessages({input});

    QCOMPARE(w.rowCount(), 1);
    // Don't create duplicates
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 1);

    // Add other messageId
    input.setMessageId(QStringLiteral("ff2"));
    input.setTimeStamp(43);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 2);

    input.setMessageId(QStringLiteral("ff3"));
    input.setTimeStamp(44);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 3);

    input.setMessageId(QStringLiteral("ff4"));
    input.setTimeStamp(45);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 4);

    input.setMessageId(QStringLiteral("ff2"));
    input.setTimeStamp(43);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 4);
}

void MessagesModelTest::shouldRemoveMessage()
{
    MessagesModel w;
    Message input;
    fillTestMessage(input);

    QSignalSpy rowInsertedSpy(&w, &MessagesModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &MessagesModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &MessagesModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &MessagesModel::rowsAboutToBeRemoved);

    const QString messageId = QStringLiteral("ff");
    input.setMessageId(messageId);
    w.addMessages({input});

    QCOMPARE(w.rowCount(), 1);
    // Remove existing message

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

    w.addMessages({input});
    QCOMPARE(w.rowCount(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,0"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,0"));
}

void MessagesModelTest::shouldRemoveNotExistingMessage()
{
    MessagesModel w;
    Message input;
    fillTestMessage(input);
    const QString messageId = QStringLiteral("ff");
    input.setMessageId(messageId);
    w.addMessages({input});

    QCOMPARE(w.rowCount(), 1);
    // Remove non-existing message
    w.deleteMessage(QStringLiteral("Bla"));
    QCOMPARE(w.rowCount(), 1);

    input.setMessageId(QStringLiteral("ff3"));
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 2);

    input.setMessageId(QStringLiteral("ff4"));
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 3);

    // Remove 3th element
    w.deleteMessage(QStringLiteral("ff3"));
    QCOMPARE(w.rowCount(), 2);
}

void MessagesModelTest::shouldDetectDateChange()
{
    MessagesModel model;
    Message first;
    first.setMessageId(QStringLiteral("first"));
    first.setTimeStamp(QDateTime(QDate(2019, 6, 7), QTime(23, 50, 50)).toMSecsSinceEpoch());
    model.addMessages({first});
    QVERIFY(model.index(0, 0).data(MessagesModel::DateDiffersFromPrevious).toBool()); // first message

    Message second;
    second.setMessageId(QStringLiteral("second"));
    second.setTimeStamp(QDateTime(QDate(2019, 6, 8), QTime(1, 2, 3)).toMSecsSinceEpoch());
    model.addMessages({second});
    QCOMPARE(model.rowCount(), 2);
    QVERIFY(model.index(1, 0).data(MessagesModel::DateDiffersFromPrevious).toBool()); // next day

    Message third;
    third.setTimeStamp(QDateTime(QDate(2019, 6, 8), QTime(1, 4, 3)).toMSecsSinceEpoch());
    third.setMessageId(QStringLiteral("third"));
    model.addMessages({third});
    QCOMPARE(model.rowCount(), 3);
    QVERIFY(!model.index(2, 0).data(MessagesModel::DateDiffersFromPrevious).toBool()); // same day
}

static QByteArrayList extractMessageIds(MessagesModel &m)
{
    QByteArrayList ret;
    const int count = m.rowCount();
    ret.reserve(count);
    for (int row = 0; row < count; ++row) {
        ret << m.index(row, 0).data(MessagesModel::MessageId).toString().toLatin1();
    }
    return ret;
}

void MessagesModelTest::shouldAddMessages()
{
    MessagesModel model;
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
    QCOMPARE(extractMessageIds(model),
             QByteArrayList() << "msgD"
                              << "msgB"
                              << "msgC"
                              << "msgA");

    messages.clear();
    messages << makeMessage("msgE", 1);
    messages << makeMessage("msgF", 3);
    messages << makeMessage("msgG", 9);
    messages << makeMessage("msgH", 5);
    input.setText(QStringLiteral("modified"));
    messages << makeMessage("msgA", 8); // update
    model.addMessages(messages);
    QCOMPARE(model.rowCount(), 8);
    QCOMPARE(extractMessageIds(model),
             QByteArrayList() << "msgE"
                              << "msgD"
                              << "msgF"
                              << "msgB"
                              << "msgH"
                              << "msgC"
                              << "msgA"
                              << "msgG");
    QCOMPARE(model.index(6, 0).data(MessagesModel::OriginalMessage).toString(), QStringLiteral("modified"));
}

void MessagesModelTest::shouldUpdateFirstMessage()
{
    MessagesModel model;
    Message input;
    fillTestMessage(input);
    model.addMessages({input});
    QCOMPARE(model.rowCount(), 1);
    input.setText(QStringLiteral("modified"));
    model.addMessages({input});
    QCOMPARE(model.index(0, 0).data(MessagesModel::OriginalMessage).toString(), QStringLiteral("modified"));
}

void MessagesModelTest::shouldAllowEditing()
{
    // GIVEN a message from me
    MessagesModel model(QStringLiteral("roomId"), Ruqola::self()->rocketChatAccount());
    RuqolaServerConfig::ServerConfigFeatureTypes settings;
    settings |= RuqolaServerConfig::ServerConfigFeatureType::AllowEditingMessage;
    settings |= RuqolaServerConfig::ServerConfigFeatureType::AllowMessageDeleting;
    Ruqola::self()->rocketChatAccount()->ruqolaServerConfig()->setServerConfigFeatureTypes(settings);
    Message input;
    fillTestMessage(input);
    model.addMessages({input});
    QCOMPARE(model.rowCount(), 1);

    // THEN
    QVERIFY(model.index(0, 0).data(MessagesModel::CanDeleteMessage).toBool());
    QVERIFY(model.index(0, 0).data(MessagesModel::CanEditMessage).toBool());

    // GIVEN a message from someone else
    input.setMessageId(QStringLiteral("msg2"));
    input.setUserId(QStringLiteral("someone_else"));
    model.addMessages({input});

    // THEN
    QVERIFY(!model.index(1, 0).data(MessagesModel::CanEditMessage).toBool());
    QVERIFY(!model.index(1, 0).data(MessagesModel::CanDeleteMessage).toBool());
}

void MessagesModelTest::shouldFindPrevNextMessage()
{
    // GIVEN an empty model
    MessagesModel model(QStringLiteral("roomId"), Ruqola::self()->rocketChatAccount());

    auto isByMe = [](const Message &msg) {
        return msg.userId() == QLatin1String("userid1");
    };

    // THEN there is no prev/next message
    QCOMPARE(model.findNextMessageAfter(QString(), isByMe).messageId(), QString());
    QCOMPARE(model.findLastMessageBefore(QString(), isByMe).messageId(), QString());
    QCOMPARE(model.findNextMessageAfter(QStringLiteral("doesnotexist"), isByMe).messageId(), QString());
    QCOMPARE(model.findLastMessageBefore(QStringLiteral("doesnotexist"), isByMe).messageId(), QString());

    // GIVEN a non-empty model
    Message input;
    fillTestMessage(input);
    QVector<Message> messages;
    auto makeMessage = [&](const char *id, const char *userId) {
        input.setMessageId(QString::fromLatin1(id));
        input.setUserId(QString::fromLatin1(userId));
        static int timestamp = 1;
        input.setTimeStamp(timestamp);
        return input;
    };
    messages << makeMessage("msgA", "userid1");
    messages << makeMessage("msgB", "userid2");
    messages << makeMessage("msgC", "userid1");
    model.addMessages(messages);

    // WHEN/THEN
    QCOMPARE(model.findLastMessageBefore(QString(), isByMe).messageId(), QStringLiteral("msgC"));
    QCOMPARE(model.findLastMessageBefore(QStringLiteral("msgC"), isByMe).messageId(), QStringLiteral("msgA"));
    QCOMPARE(model.findLastMessageBefore(QStringLiteral("msgA"), isByMe).messageId(), QString());
    QCOMPARE(model.findNextMessageAfter(QString(), isByMe).messageId(), QString());
    QCOMPARE(model.findNextMessageAfter(QStringLiteral("msgC"), isByMe).messageId(), QString());
    QCOMPARE(model.findNextMessageAfter(QStringLiteral("msgA"), isByMe).messageId(), QStringLiteral("msgC"));
}

#include "moc_messagesmodeltest.cpp"

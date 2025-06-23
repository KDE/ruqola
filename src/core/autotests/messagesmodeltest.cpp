/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagesmodeltest.h"
using namespace Qt::Literals::StringLiterals;

#include "accountmanager.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
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
    QVERIFY(!rcAccount);
    Ruqola::self()->accountManager()->addAccount(new RocketChatAccount());
    rcAccount = Ruqola::self()->rocketChatAccount();
    QVERIFY(rcAccount);
    rcAccount->settings()->setUserId("userid1"_ba);
}

void MessagesModelTest::shouldHaveDefaultValue()
{
    MessagesModel w("roomId"_ba, Ruqola::self()->rocketChatAccount());
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.searchText().isEmpty());
}

static void fillTestMessage(Message &input)
{
    input.setRoomId("room2"_ba);
    input.setText(u"message1"_s);
    input.setTimeStamp(QDateTime::currentMSecsSinceEpoch());
    input.setUsername(u"user1"_s);
    input.setUserId("userid1"_ba);
    input.setUpdatedAt(45);
    input.setEditedAt(89);
    input.setEditedByUsername(u"editeduser1"_s);
    input.setAlias(u"ali"_s);
    input.setAvatar(u"avatar1"_s);
    input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
    input.setGroupable(true);
    input.setParseUrls(true);
    input.setMessageType(Message::MessageType::NormalText);
}

void MessagesModelTest::shouldAddMessage()
{
    MessagesModel w;
    Message input;
    fillTestMessage(input);
    input.setMessageId("ff"_ba);
    w.addMessages({input});

    QCOMPARE(w.rowCount(), 1);
    // Don't create duplicates
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 1);

    // Add other messageId
    input.setMessageId("ff2"_ba);
    input.setTimeStamp(43);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 2);

    input.setMessageId("ff3"_ba);
    input.setTimeStamp(44);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 3);

    input.setMessageId("ff4"_ba);
    input.setTimeStamp(45);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 4);

    input.setMessageId("ff2"_ba);
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

    const QByteArray messageId = "ff"_ba;
    input.setMessageId(messageId);
    w.addMessages({input});

    QCOMPARE(w.rowCount(), 1);
    // Remove existing message

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,0"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,0"_s);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.deleteMessage(messageId);

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), u"0,0"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), u"0,0"_s);

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
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,0"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,0"_s);
}

void MessagesModelTest::shouldRemoveNotExistingMessage()
{
    MessagesModel w;
    Message input;
    fillTestMessage(input);
    const QByteArray messageId = "ff"_ba;
    input.setMessageId(messageId);
    w.addMessages({input});

    QCOMPARE(w.rowCount(), 1);
    // Remove non-existing message
    w.deleteMessage("Bla"_ba);
    QCOMPARE(w.rowCount(), 1);

    input.setMessageId("ff3"_ba);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 2);

    input.setMessageId("ff4"_ba);
    w.addMessages({input});
    QCOMPARE(w.rowCount(), 3);

    // Remove 3th element
    w.deleteMessage("ff3"_ba);
    QCOMPARE(w.rowCount(), 2);
}

void MessagesModelTest::shouldDetectDateChange()
{
    MessagesModel model;
    Message first;
    first.setMessageId("first"_ba);
    first.setTimeStamp(QDateTime(QDate(2019, 6, 7), QTime(20, 50, 50), QTimeZone::UTC).toMSecsSinceEpoch());
    model.addMessages({first});
    QCOMPARE(model.rowCount(), 1);
    QVERIFY(model.index(0, 0).data(MessagesModel::DateDiffersFromPrevious).toBool()); // first message

    Message second;
    second.setMessageId("second"_ba);
    second.setTimeStamp(QDateTime(QDate(2019, 6, 8), QTime(5, 2, 3), QTimeZone::UTC).toMSecsSinceEpoch());
    model.addMessages({second});
    QCOMPARE(model.rowCount(), 2);
    QVERIFY(model.index(1, 0).data(MessagesModel::DateDiffersFromPrevious).toBool()); // next day

    Message third;
    third.setTimeStamp(QDateTime(QDate(2019, 6, 8), QTime(5, 4, 3), QTimeZone::UTC).toMSecsSinceEpoch());
    third.setMessageId("third"_ba);
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
        ret << m.index(row, 0).data(MessagesModel::MessageId).toByteArray();
    }
    return ret;
}

void MessagesModelTest::shouldAddMessages()
{
    MessagesModel model;
    Message input;
    fillTestMessage(input);
    QList<Message> messages;
    auto makeMessage = [&](const char *id, qint64 timestamp) {
        input.setMessageId(QByteArray(id));
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
    input.setText(u"modified"_s);
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
    QCOMPARE(model.index(6, 0).data(MessagesModel::OriginalMessage).toString(), u"modified"_s);
}

void MessagesModelTest::shouldUpdateFirstMessage()
{
    MessagesModel model;
    Message input;
    fillTestMessage(input);
    model.addMessages({input});
    QCOMPARE(model.rowCount(), 1);
    input.setText(u"modified"_s);
    model.addMessages({input});
    QCOMPARE(model.index(0, 0).data(MessagesModel::OriginalMessage).toString(), u"modified"_s);
}

void MessagesModelTest::shouldAllowEditing()
{
    // GIVEN a message from me
    MessagesModel model("roomId"_ba, Ruqola::self()->rocketChatAccount());
    RuqolaServerConfig::ServerConfigFeatureTypes settings;
    settings |= RuqolaServerConfig::ServerConfigFeatureType::AllowEditingMessage;
    settings |= RuqolaServerConfig::ServerConfigFeatureType::AllowMessageDeleting;
    Ruqola::self()->rocketChatAccount()->ruqolaServerConfig()->setServerConfigFeatureTypes(settings);
    Message input;
    fillTestMessage(input);
    model.addMessages({input});
    QCOMPARE(model.rowCount(), 1);

    // THEN
    QVERIFY(!model.index(0, 0).data(MessagesModel::CanDeleteMessage).toBool());
    QVERIFY(model.index(0, 0).data(MessagesModel::CanEditMessage).toBool());

    // GIVEN a message from someone else
    input.setMessageId("msg2"_ba);
    input.setUserId("someone_else"_ba);
    model.addMessages({input});

    // THEN
    QVERIFY(!model.index(1, 0).data(MessagesModel::CanEditMessage).toBool());
    QVERIFY(!model.index(1, 0).data(MessagesModel::CanDeleteMessage).toBool());
}

void MessagesModelTest::shouldFindPrevNextMessage()
{
    // GIVEN an empty model
    MessagesModel model("roomId"_ba, Ruqola::self()->rocketChatAccount());

    auto isByMe = [](const Message &msg) {
        return msg.userId() == "userid1"_ba;
    };

    // THEN there is no prev/next message
    QCOMPARE(model.findNextMessageAfter(QByteArray(), isByMe).messageId(), QByteArray());
    QCOMPARE(model.findLastMessageBefore(QByteArray(), isByMe).messageId(), QByteArray());
    QCOMPARE(model.findNextMessageAfter("doesnotexist"_ba, isByMe).messageId(), QByteArray());
    QCOMPARE(model.findLastMessageBefore("doesnotexist"_ba, isByMe).messageId(), QByteArray());

    // GIVEN a non-empty model
    Message input;
    fillTestMessage(input);
    QList<Message> messages;
    auto makeMessage = [&](const QByteArray &id, const QByteArray &userId) {
        input.setMessageId(id);
        input.setUserId(userId);
        static int timestamp = 1;
        input.setTimeStamp(timestamp);
        return input;
    };
    messages << makeMessage(QByteArrayLiteral("msgA"), "userid1"_ba);
    messages << makeMessage(QByteArrayLiteral("msgB"), "userid2"_ba);
    messages << makeMessage(QByteArrayLiteral("msgC"), "userid1"_ba);
    model.addMessages(messages);

    // WHEN/THEN
    QCOMPARE(model.findLastMessageBefore(QByteArray(), isByMe).messageId(), "msgC"_ba);
    QCOMPARE(model.findLastMessageBefore(QByteArrayLiteral("msgC"), isByMe).messageId(), "msgA"_ba);
    QCOMPARE(model.findLastMessageBefore("msgA"_ba, isByMe).messageId(), QByteArray());
    QCOMPARE(model.findNextMessageAfter(QByteArray(), isByMe).messageId(), QByteArray());
    QCOMPARE(model.findNextMessageAfter("msgC"_ba, isByMe).messageId(), QByteArray());
    QCOMPARE(model.findNextMessageAfter(QByteArrayLiteral("msgA"), isByMe).messageId(), "msgC"_ba);
}

#include "moc_messagesmodeltest.cpp"

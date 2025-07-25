/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessageloggertest.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabase/localmessagelogger.h"
#include "messages/message.h"

#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(LocalMessageLoggerTest)

static QString accountName()
{
    return u"myAccount"_s;
}
static QString roomName()
{
    return u"myRoom"_s;
}
static QString otherRoomName()
{
    return u"otherRoom"_s;
}
static QString existingRoomName()
{
    return u"existingRoom"_s;
}
enum class Fields {
    MessageId,
    TimeStamp,
    UserName,
    Text,
}; // in the same order as the table

void LocalMessageLoggerTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalMessageLogger logger;
    QFile::remove(logger.dbFileName(accountName(), roomName()));
    QFile::remove(logger.dbFileName(accountName(), otherRoomName()));
    QFile::remove(logger.dbFileName(accountName(), existingRoomName()));
}

void LocalMessageLoggerTest::shouldStoreMessages()
{
    // GIVEN
    LocalMessageLogger logger;

    Message message1;
    message1.setText(QString::fromUtf8("Message text: €1"));
    message1.setUsername(QString::fromUtf8("Hervé"));
    message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 50, 50)).toMSecsSinceEpoch());
    message1.setMessageId("msg-1"_ba);
    logger.addMessage(accountName(), roomName(), message1);

    message1.setText(QString::fromUtf8("Message text: €2"));
    message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 50, 55)).toMSecsSinceEpoch());
    logger.addMessage(accountName(), roomName(), message1); // update an existing message 5s later

    Message message2;
    message2.setText(QString::fromUtf8("Message text: ßĐ"));
    message2.setUsername(QString::fromUtf8("Joe"));
    message2.setTimeStamp(QDateTime(QDate(2022, 6, 7), QTime(23, 40, 50)).toMSecsSinceEpoch()); // earlier
    message2.setMessageId("msg-2"_ba);
    logger.addMessage(accountName(), roomName(), message2);

    Message messageOtherRoom;
    messageOtherRoom.setText(QString::fromUtf8("Message other room"));
    messageOtherRoom.setUsername(QString::fromUtf8("Joe"));
    messageOtherRoom.setTimeStamp(QDateTime(QDate(2022, 6, 7), QTime(23, 30, 50)).toMSecsSinceEpoch());
    messageOtherRoom.setMessageId("msg-other-1"_ba);
    logger.addMessage(accountName(), otherRoomName(), messageOtherRoom);

    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), roomName());

    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 2);
    const QSqlRecord record0 = tableModel->record(0);
    QCOMPARE(record0.value(int(Fields::Text)).toString(), message1.text());
    QCOMPARE(record0.value(int(Fields::UserName)).toString(), message1.username());
    QCOMPARE(record0.value(int(Fields::TimeStamp)).toULongLong(), message1.timeStamp());
    const QSqlRecord record1 = tableModel->record(1);
    QCOMPARE(record1.value(int(Fields::Text)).toString(), message2.text());
    QCOMPARE(record1.value(int(Fields::UserName)).toString(), message2.username());
    QCOMPARE(record1.value(int(Fields::TimeStamp)).toULongLong(), message2.timeStamp());
}

void LocalMessageLoggerTest::shouldLoadExistingDb() // this test depends on shouldStoreMessages()
{
    // GIVEN
    LocalMessageLogger logger;
    // Copy an existing db under a new room name, so that there's not yet a QSqlDatabase for it
    QSqlDatabase::database(accountName() + u'-' + otherRoomName()).close();
    const QString srcDb = logger.dbFileName(accountName(), otherRoomName());
    const QString destDb = logger.dbFileName(accountName(), existingRoomName());
    QVERIFY(QFileInfo::exists(srcDb));
    QVERIFY(!QFileInfo::exists(destDb));
    QVERIFY(QFile::copy(srcDb, destDb));

    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), existingRoomName());

    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 1);
    const QSqlRecord record0 = tableModel->record(0);
    QCOMPARE(record0.value(int(Fields::Text)).toString(), u"Message other room"_s);
    QCOMPARE(record0.value(int(Fields::UserName)).toString(), u"Joe"_s);
}

void LocalMessageLoggerTest::shouldDeleteMessages() // this test depends on shouldStoreMessages()
{
    // GIVEN
    LocalMessageLogger logger;
    const QString messageId = (u"msg-other-1"_s);

    // WHEN
    logger.deleteMessage(accountName(), otherRoomName(), messageId);

    // THEN
    auto tableModel = logger.createMessageModel(accountName(), otherRoomName());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 0);
}

void LocalMessageLoggerTest::shouldReturnNullIfDoesNotExist()
{
    // GIVEN
    LocalMessageLogger logger;
    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), u"does not exist"_s);
    // THEN
    QVERIFY(!tableModel);
}

void LocalMessageLoggerTest::shouldDefaultValues()
{
    LocalMessageLogger logger;
    QCOMPARE(logger.schemaDatabaseStr(), u"CREATE TABLE LOGS (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, userName TEXT, text TEXT)"_s);
}

#include "moc_localmessageloggertest.cpp"

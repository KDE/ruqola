/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessageloggertest.h"
#include "localmessagelogger.h"
#include "messages/message.h"

#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(LocalMessageLoggerTest)

static QString accountName()
{
    return QStringLiteral("myAccount");
}
static QString roomName()
{
    return QStringLiteral("myRoom");
}
static QString otherRoomName()
{
    return QStringLiteral("otherRoom");
}
static QString existingRoomName()
{
    return QStringLiteral("existingRoom");
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
    message1.setMessageId(QStringLiteral("msg-1"));
    logger.addMessage(accountName(), roomName(), message1);

    message1.setText(QString::fromUtf8("Message text: €2"));
    message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 50, 55)).toMSecsSinceEpoch());
    logger.addMessage(accountName(), roomName(), message1); // update an existing message 5s later

    Message message2;
    message2.setText(QString::fromUtf8("Message text: ßĐ"));
    message2.setUsername(QString::fromUtf8("Joe"));
    message2.setTimeStamp(QDateTime(QDate(2022, 6, 7), QTime(23, 40, 50)).toMSecsSinceEpoch()); // earlier
    message2.setMessageId(QStringLiteral("msg-2"));
    logger.addMessage(accountName(), roomName(), message2);

    Message messageOtherRoom;
    messageOtherRoom.setText(QString::fromUtf8("Message other room"));
    messageOtherRoom.setUsername(QString::fromUtf8("Joe"));
    messageOtherRoom.setTimeStamp(QDateTime(QDate(2022, 6, 7), QTime(23, 30, 50)).toMSecsSinceEpoch());
    messageOtherRoom.setMessageId(QStringLiteral("msg-other-1"));
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
    QSqlDatabase::database(accountName() + QLatin1Char('-') + otherRoomName()).close();
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
    QCOMPARE(record0.value(int(Fields::Text)).toString(), QStringLiteral("Message other room"));
    QCOMPARE(record0.value(int(Fields::UserName)).toString(), QStringLiteral("Joe"));
}

void LocalMessageLoggerTest::shouldDeleteMessages() // this test depends on shouldStoreMessages()
{
    // GIVEN
    LocalMessageLogger logger;
    const QString messageId = (QStringLiteral("msg-other-1"));

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
    auto tableModel = logger.createMessageModel(accountName(), QStringLiteral("does not exist"));
    // THEN
    QVERIFY(!tableModel);
}

/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagedatabasetest.h"
#include "localdatabase/localmessagedatabase.h"
#include "messages/message.h"

#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(LocalMessageDatabaseTest)

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
    Json,
}; // in the same order as the table

void LocalMessageDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalMessageDatabase logger;
    QFile::remove(logger.dbFileName(accountName(), roomName()));
    QFile::remove(logger.dbFileName(accountName(), otherRoomName()));
    QFile::remove(logger.dbFileName(accountName(), existingRoomName()));
}

void LocalMessageDatabaseTest::shouldStoreMessages()
{
    // GIVEN
    LocalMessageDatabase logger;

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
    QCOMPARE(record0.value(int(Fields::Json)).toByteArray(), Message::serialize(message1, false));
    QCOMPARE(record0.value(int(Fields::TimeStamp)).toULongLong(), message1.timeStamp());
    const QSqlRecord record1 = tableModel->record(1);
    QCOMPARE(record1.value(int(Fields::Json)).toByteArray(), Message::serialize(message2, false));
    QCOMPARE(record1.value(int(Fields::TimeStamp)).toULongLong(), message2.timeStamp());
}

void LocalMessageDatabaseTest::shouldLoadExistingDb() // this test depends on shouldStoreMessages()
{
    // GIVEN
    LocalMessageDatabase logger;
    // Copy an existing db under a new room name, so that there's not yet a QSqlDatabase for it
    QSqlDatabase::database(accountName() + QLatin1Char('-') + otherRoomName()).close();
    const QString srcDb = logger.dbFileName(accountName(), otherRoomName());
    const QString destDb = logger.dbFileName(accountName(), existingRoomName());
    QVERIFY(QFileInfo::exists(srcDb));
    QVERIFY(!QFileInfo::exists(destDb));
    QVERIFY(QFile::copy(srcDb, destDb));

    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), existingRoomName());
    qDebug() << " accountName() " << accountName() << " existingRoomName() " << existingRoomName();

    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 1);
    const QSqlRecord record0 = tableModel->record(0);
    QCOMPARE(record0.value(int(Fields::Json)).toString(), QStringLiteral("Message other room"));
}

void LocalMessageDatabaseTest::shouldDeleteMessages() // this test depends on shouldStoreMessages()
{
    // GIVEN
    LocalMessageDatabase logger;
    const QString messageId = (QStringLiteral("msg-other-1"));

    // WHEN
    logger.deleteMessage(accountName(), otherRoomName(), messageId);

    // THEN
    auto tableModel = logger.createMessageModel(accountName(), otherRoomName());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 0);
}

void LocalMessageDatabaseTest::shouldReturnNullIfDoesNotExist()
{
    // GIVEN
    LocalMessageDatabase logger;
    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), QStringLiteral("does not exist"));
    // THEN
    QVERIFY(!tableModel);
}

void LocalMessageDatabaseTest::shouldExtractMessages()
{
    // GIVEN
    LocalMessageDatabase logger;
    for (int i = 0; i < 20; ++i) {
        Message message1;
        message1.setText(QString::fromUtf8("Message text: %1").arg(i));
        message1.setUsername(QString::fromUtf8("Hervé %1").arg(i));
        message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 50 + i, 50)).toMSecsSinceEpoch());
        message1.setMessageId(QStringLiteral("msg-%1").arg(i));
        logger.addMessage(accountName(), roomName(), message1);
    }
    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), roomName());
    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 20);
}

void LocalMessageDatabaseTest::shouldExtractSpecificNumberOfMessages_data()
{
    QTest::addColumn<qint64>("startId");
    QTest::addColumn<qint64>("endId");
    QTest::addColumn<qint64>("numberElement");
    QTest::addColumn<qint64>("result");

    QTest::addRow("test1") << (qint64)-1 << (qint64)-1 << (qint64)5 << (qint64)5;

    QTest::addRow("ask-more-elements") << (qint64)-1 << (qint64)-1 << (qint64)25 << (qint64)20;
    QTest::addRow("ask-equal-elements") << (qint64)-1 << (qint64)-1 << (qint64)20 << (qint64)20;
    QTest::addRow("ask-no-element") << (qint64)-1 << (qint64)-1 << (qint64)0 << (qint64)0;

    // Time stand start
    QTest::addRow("timestand-start1") << (qint64)1623100790000 << (qint64)-1 << (qint64)0 << (qint64)0;
    QTest::addRow("timestand-start2") << (qint64)1623100790000 << (qint64)-1 << (qint64)1 << (qint64)1;
    QTest::addRow("timestand-start3") << (qint64)1623100790000 << (qint64)-1 << (qint64)2 << (qint64)2;
    QTest::addRow("timestand-start4") << (qint64)1623100790000 << (qint64)-1 << (qint64)10 << (qint64)2;
    QTest::addRow("timestand-start5") << (qint64)1623100790000 << (qint64)-1 << (qint64)3 << (qint64)2;
}

void LocalMessageDatabaseTest::shouldExtractSpecificNumberOfMessages()
{
    QFETCH(qint64, startId);
    QFETCH(qint64, endId);
    QFETCH(qint64, numberElement);
    QFETCH(qint64, result);

    // GIVEN
    LocalMessageDatabase logger;
    for (int i = 0; i < 20; ++i) {
        Message message1;
        message1.setText(QString::fromUtf8("Message text: %1").arg(i));
        message1.setUsername(QString::fromUtf8("Hervé %1").arg(i));
        message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 1 + i, 50)).toMSecsSinceEpoch());
        message1.setMessageId(QStringLiteral("msg-%1").arg(i));
        logger.addMessage(accountName(), roomName(), message1);
    }
    // WHEN
    const QVector<Message> messages = logger.loadMessages(accountName(), roomName(), startId, endId, numberElement);

    // THEN
    QCOMPARE(messages.count(), result);
}
#include "moc_localmessagedatabasetest.cpp"

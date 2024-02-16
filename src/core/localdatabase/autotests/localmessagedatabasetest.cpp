/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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
    QCOMPARE(tableModel->rowCount(), 0);
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

    QTest::addRow("test1") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(5) << static_cast<qint64>(5);

    QTest::addRow("ask-more-elements") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(25) << static_cast<qint64>(20);
    QTest::addRow("ask-equal-elements") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(20) << static_cast<qint64>(20);
    QTest::addRow("ask-no-element") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(0) << static_cast<qint64>(0);

    // Time stand start
    QTest::addRow("timestand-start1") << static_cast<qint64>(1623100790000) << static_cast<qint64>(-1) << static_cast<qint64>(0) << static_cast<qint64>(0);
    QTest::addRow("timestand-start2") << static_cast<qint64>(1623100790000) << static_cast<qint64>(-1) << static_cast<qint64>(1) << static_cast<qint64>(1);
    QTest::addRow("timestand-start3") << static_cast<qint64>(1623100790000) << static_cast<qint64>(-1) << static_cast<qint64>(2) << static_cast<qint64>(2);
    QTest::addRow("timestand-start4") << static_cast<qint64>(1623100790000) << static_cast<qint64>(-1) << static_cast<qint64>(10) << static_cast<qint64>(2);
    QTest::addRow("timestand-start5") << static_cast<qint64>(1623100790000) << static_cast<qint64>(-1) << (qint64)3 << static_cast<qint64>(2);

    // Start 1623099710000 end 1623100850000
    QTest::addRow("timestand-start1-end1") << static_cast<qint64>(1623099710000) << static_cast<qint64>(1623099710000) << static_cast<qint64>(5)
                                           << static_cast<qint64>(1);
    QTest::addRow("timestand-start1-end2") << static_cast<qint64>(1623099710000) << static_cast<qint64>(1623100850000) << static_cast<qint64>(30)
                                           << static_cast<qint64>(20);

    // End
    QTest::addRow("timestand-end1") << static_cast<qint64>(-1) << static_cast<qint64>(1623099710000) << static_cast<qint64>(5) << static_cast<qint64>(1);
    QTest::addRow("timestand-end3") << static_cast<qint64>(-1) << (qint64)(QDateTime(QDate(2021, 6, 7), QTime(23, 1 + 3, 50)).toMSecsSinceEpoch())
                                    << static_cast<qint64>(5) << (qint64)4;
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
    const QList<Message> messages = logger.loadMessages(accountName(), roomName(), startId, endId, numberElement);

    // THEN
    QCOMPARE(messages.count(), result);
}

void LocalMessageDatabaseTest::shouldGenerateQuery()
{
    QFETCH(qint64, startId);
    QFETCH(qint64, endId);
    QFETCH(qint64, numberElement);
    QFETCH(QString, result);

    // GIVEN
    // WHEN
    const QString queryStr = LocalMessageDatabase::generateQueryStr(startId, endId, numberElement);

    // THEN
    QCOMPARE(queryStr, result);
}

void LocalMessageDatabaseTest::shouldGenerateQuery_data()
{
    QTest::addColumn<qint64>("startId");
    QTest::addColumn<qint64>("endId");
    QTest::addColumn<qint64>("numberElement");
    QTest::addColumn<QString>("result");

    QTest::addRow("test1") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(5)
                           << QStringLiteral("SELECT * FROM MESSAGES ORDER BY timestamp DESC LIMIT :limit");
    QTest::addRow("test2") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(-1)
                           << QStringLiteral("SELECT * FROM MESSAGES ORDER BY timestamp DESC");
    QTest::addRow("test3") << static_cast<qint64>(5) << static_cast<qint64>(-1) << static_cast<qint64>(-1)
                           << QStringLiteral("SELECT * FROM MESSAGES WHERE timestamp >= :startId ORDER BY timestamp DESC");
    QTest::addRow("test4") << static_cast<qint64>(-1) << static_cast<qint64>(5) << static_cast<qint64>(-1)
                           << QStringLiteral("SELECT * FROM MESSAGES WHERE timestamp <= :endId ORDER BY timestamp DESC");
    QTest::addRow("test5") << static_cast<qint64>(5) << static_cast<qint64>(5) << static_cast<qint64>(-1)
                           << QStringLiteral("SELECT * FROM MESSAGES WHERE timestamp >= :startId AND timestamp <= :endId ORDER BY timestamp DESC");
    QTest::addRow("test6") << static_cast<qint64>(5) << static_cast<qint64>(5) << static_cast<qint64>(30)
                           << QStringLiteral("SELECT * FROM MESSAGES WHERE timestamp >= :startId AND timestamp <= :endId ORDER BY timestamp DESC LIMIT :limit");
}

void LocalMessageDatabaseTest::shouldVerifyDbFileName()
{
    LocalMessageDatabase accountDataBase;
    QCOMPARE(accountDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/database/messages/myAccount/myAccount.sqlite"));
}

#include "moc_localmessagedatabasetest.cpp"

/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagesdatabasetest.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabase/localmessagesdatabase.h"
#include "messages/message.h"

#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(LocalMessagesDatabaseTest)

static QString accountName()
{
    return u"myAccount"_s;
}
static QByteArray roomId()
{
    return "myRoom"_ba;
}
static QByteArray otherRoomId()
{
    return "otherRoom"_ba;
}
static QByteArray existingRoomId()
{
    return "existingRoom"_ba;
}
static QByteArray batchRoomId()
{
    return "batchRoom"_ba;
}
static QByteArray emptyBatchRoomId()
{
    return "emptyBatchRoom"_ba;
}
enum class Fields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table

void LocalMessagesDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    const LocalMessagesDatabase logger;
    QFile::remove(logger.dbFileName(accountName(), roomId()));
    QFile::remove(logger.dbFileName(accountName(), otherRoomId()));
    QFile::remove(logger.dbFileName(accountName(), existingRoomId()));
    QFile::remove(logger.dbFileName(accountName(), batchRoomId()));
    QFile::remove(logger.dbFileName(accountName(), emptyBatchRoomId()));
}

void LocalMessagesDatabaseTest::shouldStoreMessages()
{
    // GIVEN
    LocalMessagesDatabase logger;

    Message message1;
    message1.setText(QString::fromUtf8("Message text: €1"));
    message1.setUsername(QString::fromUtf8("Hervé"));
    message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 50, 50)).toMSecsSinceEpoch());
    message1.setMessageId("msg-1"_ba);
    logger.addMessage(accountName(), roomId(), message1);

    message1.setText(QString::fromUtf8("Message text: €2"));
    message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 50, 55)).toMSecsSinceEpoch());
    logger.addMessage(accountName(), roomId(), message1); // update an existing message 5s later

    Message message2;
    message2.setText(QString::fromUtf8("Message text: ßĐ"));
    message2.setUsername(QString::fromUtf8("Joe"));
    message2.setTimeStamp(QDateTime(QDate(2022, 6, 7), QTime(23, 40, 50)).toMSecsSinceEpoch()); // earlier
    message2.setMessageId("msg-2"_ba);
    logger.addMessage(accountName(), roomId(), message2);

    Message messageOtherRoom;
    messageOtherRoom.setText(QString::fromUtf8("Message other room"));
    messageOtherRoom.setUsername(QString::fromUtf8("Joe"));
    messageOtherRoom.setTimeStamp(QDateTime(QDate(2022, 6, 7), QTime(23, 30, 50)).toMSecsSinceEpoch());
    messageOtherRoom.setMessageId("msg-other-1"_ba);
    logger.addMessage(accountName(), otherRoomId(), messageOtherRoom);

    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), roomId());

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

void LocalMessagesDatabaseTest::shouldLoadExistingDb() // this test depends on shouldStoreMessages()
{
    // GIVEN
    const LocalMessagesDatabase logger;
    // Copy an existing db under a new room name, so that there's not yet a QSqlDatabase for it
    QSqlDatabase::database(accountName() + u'-' + QString::fromLatin1(otherRoomId())).close();
    const QString srcDb = logger.dbFileName(accountName(), otherRoomId());
    const QString destDb = logger.dbFileName(accountName(), existingRoomId());
    QVERIFY(QFileInfo::exists(srcDb));
    QVERIFY(!QFileInfo::exists(destDb));
    QVERIFY(QFile::copy(srcDb, destDb));

    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), existingRoomId());
    qDebug() << " accountName() " << accountName() << " existingRoomName() " << existingRoomId();

    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 0);
}

void LocalMessagesDatabaseTest::shouldDeleteMessages() // this test depends on shouldStoreMessages()
{
    // GIVEN
    LocalMessagesDatabase logger;
    const QString messageId = (u"msg-other-1"_s);

    // WHEN
    logger.deleteMessage(accountName(), otherRoomId(), messageId);

    // THEN
    auto tableModel = logger.createMessageModel(accountName(), otherRoomId());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 0);
}

void LocalMessagesDatabaseTest::shouldReturnNullIfDoesNotExist()
{
    // GIVEN
    const LocalMessagesDatabase logger;
    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), "does not exist"_ba);
    // THEN
    QVERIFY(!tableModel);
}

void LocalMessagesDatabaseTest::shouldExtractMessages()
{
    // GIVEN
    LocalMessagesDatabase logger;
    for (int i = 0; i < 20; ++i) {
        Message message1;
        message1.setText(QString::fromUtf8("Message text: %1").arg(i));
        message1.setUsername(QString::fromUtf8("Hervé %1").arg(i));
        message1.setTimeStamp(QDateTime(QDate(2021, 6, 7), QTime(23, 50 + i, 50)).toMSecsSinceEpoch());
        message1.setMessageId(u"msg-%1"_s.arg(i).toLatin1());
        logger.addMessage(accountName(), roomId(), message1);
    }
    // WHEN
    auto tableModel = logger.createMessageModel(accountName(), roomId());
    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 20);
}

void LocalMessagesDatabaseTest::shouldExtractSpecificNumberOfMessages_data()
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
    QTest::addRow("timestand-start4") << static_cast<qint64>(1623107990000) << static_cast<qint64>(-1) << static_cast<qint64>(10) << static_cast<qint64>(2);
    QTest::addRow("timestand-start5") << static_cast<qint64>(1623107990000) << static_cast<qint64>(-1) << (qint64)3 << static_cast<qint64>(2);

    // Start 1623099710000 end 1623100850000
    QTest::addRow("timestand-start1-end1") << static_cast<qint64>(1623108050000) << static_cast<qint64>(1623108050100) << static_cast<qint64>(5)
                                           << static_cast<qint64>(1);
    QTest::addRow("timestand-start1-end2") << static_cast<qint64>(1623106910000) << static_cast<qint64>(1623108050100) << static_cast<qint64>(30)
                                           << static_cast<qint64>(20);

    // End
    QTest::addRow("timestand-end1") << static_cast<qint64>(-1) << static_cast<qint64>(1623106910000) << static_cast<qint64>(5) << static_cast<qint64>(1);
    QTest::addRow("timestand-end3") << static_cast<qint64>(-1)
                                    << (qint64)(QDateTime(QDate(2021, 6, 7), QTime(23, 1 + 3, 50), QTimeZone::UTC).toMSecsSinceEpoch())
                                    << static_cast<qint64>(5) << (qint64)4;
}

void LocalMessagesDatabaseTest::shouldExtractSpecificNumberOfMessages()
{
    QFETCH(qint64, startId);
    QFETCH(qint64, endId);
    QFETCH(qint64, numberElement);
    QFETCH(qint64, result);

    // GIVEN
    LocalMessagesDatabase logger;
    for (int i = 0; i < 20; ++i) {
        Message message1;
        message1.setText(QString::fromUtf8("Message text: %1").arg(i));
        message1.setUsername(QString::fromUtf8("Hervé %1").arg(i));
        const auto value = QDateTime(QDate(2021, 6, 7), QTime(23, 1 + i, 50), QTimeZone::UTC).toMSecsSinceEpoch();
        message1.setTimeStamp(value);
        message1.setMessageId(u"msg-%1"_s.arg(i).toLatin1());
        logger.addMessage(accountName(), roomId(), message1);
    }
    // WHEN
    const QList<Message> messages = logger.loadMessages(accountName(), roomId(), startId, endId, numberElement);

    // THEN
    QCOMPARE(messages.count(), result);
}

void LocalMessagesDatabaseTest::shouldGenerateQuery()
{
    QFETCH(qint64, startId);
    QFETCH(qint64, endId);
    QFETCH(qint64, numberElement);
    QFETCH(QString, result);

    // GIVEN
    // WHEN
    const QString queryStr = LocalMessagesDatabase::generateQueryStr(startId, endId, numberElement);

    // THEN
    QCOMPARE(queryStr, result);
}

void LocalMessagesDatabaseTest::shouldGenerateQuery_data()
{
    QTest::addColumn<qint64>("startId");
    QTest::addColumn<qint64>("endId");
    QTest::addColumn<qint64>("numberElement");
    QTest::addColumn<QString>("result");

    QTest::addRow("test1") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(5)
                           << u"SELECT * FROM MESSAGES ORDER BY timestamp DESC LIMIT :limit"_s;
    QTest::addRow("test2") << static_cast<qint64>(-1) << static_cast<qint64>(-1) << static_cast<qint64>(-1)
                           << u"SELECT * FROM MESSAGES ORDER BY timestamp DESC"_s;
    QTest::addRow("test3") << static_cast<qint64>(5) << static_cast<qint64>(-1) << static_cast<qint64>(-1)
                           << u"SELECT * FROM MESSAGES WHERE timestamp >= :startId ORDER BY timestamp DESC"_s;
    QTest::addRow("test4") << static_cast<qint64>(-1) << static_cast<qint64>(5) << static_cast<qint64>(-1)
                           << u"SELECT * FROM MESSAGES WHERE timestamp <= :endId ORDER BY timestamp DESC"_s;
    QTest::addRow("test5") << static_cast<qint64>(5) << static_cast<qint64>(5) << static_cast<qint64>(-1)
                           << u"SELECT * FROM MESSAGES WHERE timestamp >= :startId AND timestamp <= :endId ORDER BY timestamp DESC"_s;
    QTest::addRow("test6") << static_cast<qint64>(5) << static_cast<qint64>(5) << static_cast<qint64>(30)
                           << u"SELECT * FROM MESSAGES WHERE timestamp >= :startId AND timestamp <= :endId ORDER BY timestamp DESC LIMIT :limit"_s;
}

void LocalMessagesDatabaseTest::shouldVerifyDbFileName()
{
    const LocalMessagesDatabase accountDataBase;
    QCOMPARE(accountDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/database/messages/myAccount/myAccount.sqlite"_s);
}

void LocalMessagesDatabaseTest::shouldStoreMessagesInOneBatch()
{
    // GIVEN
    LocalMessagesDatabase database;

    Message message1;
    message1.setText(u"batch 1"_s);
    message1.setUsername(u"Joe"_s);
    message1.setTimeStamp(QDateTime(QDate(2023, 1, 1), QTime(10, 0, 0)).toMSecsSinceEpoch());
    message1.setMessageId("batch-1"_ba);

    Message message2;
    message2.setText(u"batch 2"_s);
    message2.setUsername(u"Joe"_s);
    message2.setTimeStamp(QDateTime(QDate(2023, 1, 1), QTime(10, 0, 5)).toMSecsSinceEpoch());
    message2.setMessageId("batch-2"_ba);

    // Same messageId as message1, later in the very same batch: the insert-or-replace must still
    // win inside the single transaction.
    Message message1Updated = message1;
    message1Updated.setText(u"batch 1 updated"_s);
    message1Updated.setTimeStamp(QDateTime(QDate(2023, 1, 1), QTime(10, 0, 10)).toMSecsSinceEpoch());

    // WHEN
    database.addMessages(accountName(), batchRoomId(), {message1, message2, message1Updated});

    // THEN
    auto tableModel = database.createMessageModel(accountName(), batchRoomId());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 2);

    // loadMessages() sorts by timestamp, descending.
    const QList<Message> messages = database.loadMessages(accountName(), batchRoomId());
    QCOMPARE(messages.count(), 2);
    QCOMPARE(messages.at(0).messageId(), message1Updated.messageId());
    QCOMPARE(messages.at(0).text(), message1Updated.text());
    QCOMPARE(messages.at(0).timeStamp(), message1Updated.timeStamp());
    QCOMPARE(messages.at(1).messageId(), message2.messageId());
    QCOMPARE(messages.at(1).text(), message2.text());
}

void LocalMessagesDatabaseTest::shouldIgnoreEmptyMessageBatch()
{
    // GIVEN
    LocalMessagesDatabase database;
    const QString fileName = database.dbFileName(accountName(), emptyBatchRoomId());
    QVERIFY(!QFileInfo::exists(fileName));

    // WHEN
    database.addMessages(accountName(), emptyBatchRoomId(), {});

    // THEN an empty batch must not even create the database file
    QVERIFY(!QFileInfo::exists(fileName));
}

#include "moc_localmessagesdatabasetest.cpp"

/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroomsdatabasetest.h"

#include "localdatabase/localroomsdatabase.h"
#include "room.h"
#include "ruqola_autotest_helper.h"
#include <QFile>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>

QTEST_GUILESS_MAIN(LocalRoomsDatabaseTest)
using namespace Qt::Literals::StringLiterals;
static QString accountName()
{
    return u"myAccount"_s;
}

static QString otherAccountName()
{
    return u"myOtherAccount"_s;
}

enum class RoomFields {
    RoomId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalRoomsDatabaseTest::LocalRoomsDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalRoomsDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalRoomsDatabase roomDatabase;
    QFile::remove(roomDatabase.dbFileName(accountName()));
    QFile::remove(roomDatabase.dbFileName(otherAccountName()));
}

void LocalRoomsDatabaseTest::shouldDefaultValues()
{
    LocalRoomsDatabase roomDatabase;
    QCOMPARE(roomDatabase.schemaDatabaseStr(), u"CREATE TABLE ROOMS (roomId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)"_s);
}

void LocalRoomsDatabaseTest::shouldVerifyDbFileName()
{
    LocalRoomsDatabase roomDatabase;
    QCOMPARE(roomDatabase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/database/rooms/myAccount/myAccount.sqlite"_s);
}

void LocalRoomsDatabaseTest::shouldStoreRoomsSettings()
{
    {
        const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/rooms/direct-room.json"_L1;
        const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);
        LocalRoomsDatabase roomDataBase;
        Room r;
        r.parseSubscriptionRoom(fields);
        const QByteArray ba = Room::serialize(&r, false);
        roomDataBase.updateRoom(accountName(), &r);

        // WHEN
        const QByteArray getInfo = roomDataBase.jsonRoom(accountName(), QString::fromLatin1(r.roomId()));

        // THEN
        QCOMPARE(getInfo, ba);
        Room r1;
        const auto doc = QJsonDocument::fromJson(getInfo);
        Room::deserialize(&r1, doc.object());
        QCOMPARE(r1, r);
    }
}

void LocalRoomsDatabaseTest::shouldStoreRooms()
{
    // GIVEN
    LocalRoomsDatabase logger;

    Room room1;
    room1.setRoomId("room-1"_ba);
    room1.setAnnouncement(QString::fromUtf8("Hervé"));
    room1.setChannelType(Room::RoomType::Private);
    room1.setOpen(false);
    room1.setName(QString::fromUtf8("Marché"));
    room1.setUpdatedAt(QDateTime(QDate(2022, 6, 7), QTime(23, 40, 50)).toMSecsSinceEpoch());
    logger.updateRoom(otherAccountName(), &room1);

    room1.setAnnouncement(QString::fromUtf8("Room Announcement text: €2"));
    room1.setUpdatedAt(QDateTime(QDate(2021, 6, 7), QTime(23, 50, 55)).toMSecsSinceEpoch());
    logger.updateRoom(otherAccountName(), &room1); // update an existing message 5s later

    Room room2;
    room2.setRoomId("room-2"_ba);

    room2.setAnnouncement(QString::fromUtf8("Foo"));
    room2.setChannelType(Room::RoomType::Private);
    room2.setOpen(true);
    room2.setName(QString::fromUtf8("Noël"));
    room2.setUpdatedAt(QDateTime(QDate(2022, 6, 7), QTime(23, 40, 50)).toMSecsSinceEpoch()); // earlier
    logger.updateRoom(otherAccountName(), &room2);

    logger.updateRoom(otherAccountName(), &room2);
#if 0
    Room room-other;
    messageOtherRoom.setText(QString::fromUtf8("Message other room"));
    messageOtherRoom.setUsername(QString::fromUtf8("Joe"));
    messageOtherRoom.setTimeStamp(QDateTime(QDate(2022, 6, 7), QTime(23, 30, 50)).toMSecsSinceEpoch());
    messageOtherRoom.setMessageId("msg-other-1"_ba);
    logger.updateRoom(otherAccountName(), otherRoomName(), messageOtherRoom);
#endif
    // WHEN
    auto tableModel = logger.createRoomsModel(otherAccountName());

    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 2);
    const QSqlRecord record0 = tableModel->record(0);
    QCOMPARE(record0.value(int(RoomFields::Json)).toByteArray(), Room::serialize(&room1, false));
    QCOMPARE(record0.value(int(RoomFields::TimeStamp)).toULongLong(), room1.updatedAt());
    const QSqlRecord record1 = tableModel->record(1);
    QCOMPARE(record1.value(int(RoomFields::Json)).toByteArray(), Room::serialize(&room2, false));
    QCOMPARE(record1.value(int(RoomFields::TimeStamp)).toULongLong(), room2.updatedAt());
}

void LocalRoomsDatabaseTest::shouldDeleteRooms() // this test depends on shouldStoreMessages()
{
    // GIVEN
    LocalRoomsDatabase logger;
    const QString roomId = (u"roomId-other-1"_s);

    // WHEN
    logger.deleteRoom(accountName(), roomId);
#if 0
    // THEN
    auto tableModel = logger.createRoomsModel(accountName());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 0);
#endif
}

#include "moc_localroomsdatabasetest.cpp"

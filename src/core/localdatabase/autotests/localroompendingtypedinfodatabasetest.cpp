/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroompendingtypedinfodatabasetest.h"
#include "localdatabase/localroompendingtypedinfodatabase.h"
#include <QJsonObject>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(LocalRoomPendingTypedInfoDatabaseTest)
using namespace Qt::Literals::StringLiterals;
static QString accountName()
{
    return u"myAccount"_s;
}

static QString otherAccountName()
{
    return u"myOtherAccount"_s;
}

enum class RoomPendingTypedFields {
    RoomId,
    Json,
}; // in the same order as the table

LocalRoomPendingTypedInfoDatabaseTest::LocalRoomPendingTypedInfoDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalRoomPendingTypedInfoDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    LocalRoomPendingTypedInfoDatabase roomPendingTypedInfoDataBase;
    QFile::remove(roomPendingTypedInfoDataBase.dbFileName(accountName()));
    QFile::remove(roomPendingTypedInfoDataBase.dbFileName(otherAccountName()));
}

void LocalRoomPendingTypedInfoDatabaseTest::shouldDefaultValues()
{
    LocalRoomPendingTypedInfoDatabase roomPendingTypedInfoDataBase;
    QCOMPARE(roomPendingTypedInfoDataBase.schemaDatabaseStr(), u"CREATE TABLE ROOMPENDINGTYPED (roomId TEXT PRIMARY KEY NOT NULL, json TEXT)"_s);
}

void LocalRoomPendingTypedInfoDatabaseTest::shouldVerifyDbFileName()
{
    LocalRoomPendingTypedInfoDatabase roomPendingTypedInfoDataBase;
    QCOMPARE(roomPendingTypedInfoDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/database/roompendingtypedinfo/myAccount/myAccount.sqlite"_s);
}

void LocalRoomPendingTypedInfoDatabaseTest::shouldStoreRoomPendingTypedInfo()
{
    // GIVEN
    LocalRoomPendingTypedInfoDatabase logger;

    QByteArray roomId = "foo1"_ba;
    AccountRoomSettings::PendingTypedInfo info1;
    info1.messageIdBeingEdited = "foo"_ba;
    info1.quotePermalink = u"bla"_s;
    logger.updateRoomPendingTypedInfo(otherAccountName(), roomId, info1);

    // Update PendingTypedInfo
    info1.scrollbarPosition = 5;
    logger.updateRoomPendingTypedInfo(otherAccountName(), roomId, info1);

    QByteArray roomId2 = "foo2"_ba;
    AccountRoomSettings::PendingTypedInfo info2;
    info2.messageIdBeingEdited = "foo1"_ba;
    info2.quotePermalink = u"bla1"_s;
    info2.scrollbarPosition = 5;
    logger.updateRoomPendingTypedInfo(otherAccountName(), roomId2, info2);

    QByteArray roomId3 = "foo3"_ba;
    AccountRoomSettings::PendingTypedInfo info3;
    info3.messageIdBeingEdited = "foo5"_ba;
    info3.quotePermalink = u"bla-kde"_s;
    info3.scrollbarPosition = 5;
    info3.quoteText = u"test1"_s;
    logger.updateRoomPendingTypedInfo(otherAccountName(), roomId3, info2);

    // WHEN
    auto tableModel = logger.createRoomsModel(otherAccountName());

    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 3);
    const QSqlRecord record0 = tableModel->record(0);
    QCOMPARE(record0.value(int(RoomPendingTypedFields::Json)).toByteArray(),
             QJsonDocument(AccountRoomSettings::PendingTypedInfo::serialize(info1)).toJson(QJsonDocument::Compact));
    QCOMPARE(record0.value(int(RoomPendingTypedFields::RoomId)).toByteArray(), roomId);

    const QSqlRecord record1 = tableModel->record(1);
    QCOMPARE(record1.value(int(RoomPendingTypedFields::Json)).toByteArray(),
             QJsonDocument(AccountRoomSettings::PendingTypedInfo::serialize(info2)).toJson(QJsonDocument::Compact));
    QCOMPARE(record1.value(int(RoomPendingTypedFields::RoomId)).toByteArray(), roomId2);
}

void LocalRoomPendingTypedInfoDatabaseTest::shouldDeleteRoomPendingTypedInfo() // this test depends on shouldStoreRoomPendingTypedInfo()
{
    // GIVEN
    LocalRoomPendingTypedInfoDatabase logger;
    const QByteArray roomId = "foo2"_ba;

    // WHEN
    logger.deleteRoomPendingTypedInfo(otherAccountName(), roomId);

    // THEN
    auto tableModel = logger.createRoomsModel(otherAccountName());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 2);
}

void LocalRoomPendingTypedInfoDatabaseTest::shouldDeleteRoomPendingTypedInfoInvalidRoomId() // this test depends on shouldStoreRoomPendingTypedInfo()
{
    // GIVEN
    LocalRoomPendingTypedInfoDatabase logger;
    const QByteArray roomId = "foo2"_ba;

    // WHEN
    logger.deleteRoomPendingTypedInfo(otherAccountName(), roomId);

    // THEN
    auto tableModel = logger.createRoomsModel(otherAccountName());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 2);
}

void LocalRoomPendingTypedInfoDatabaseTest::shouldExtractJsonFromDatabase()
{
    {
        LocalRoomPendingTypedInfoDatabase logger;
        QCOMPARE(logger.loadRoomPendingTypedInfo(otherAccountName()).count(), 2);
    }
    {
        LocalRoomPendingTypedInfoDatabase logger;
        QCOMPARE(logger.loadRoomPendingTypedInfo(accountName()).count(), 0);
    }
}

#include "moc_localroompendingtypedinfodatabasetest.cpp"

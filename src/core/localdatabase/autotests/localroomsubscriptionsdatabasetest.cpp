/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroomsubscriptionsdatabasetest.h"
#include "localdatabase/localroomsubscriptionsdatabase.h"
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(LocalRoomSubscriptionsDatabaseTest)
using namespace Qt::Literals::StringLiterals;
static QString accountName()
{
    return u"myAccount"_s;
}

static QString otherAccountName()
{
    return u"myOtherAccount"_s;
}

enum class RoomSubscriptionFields {
    SubscriptionId,
    RoomId,
}; // in the same order as the table

LocalRoomSubscriptionsDatabaseTest::LocalRoomSubscriptionsDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalRoomSubscriptionsDatabaseTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);

    // Clean up after previous runs
    const LocalRoomSubscriptionsDatabase roomSubscriptionDataBase;
    QFile::remove(roomSubscriptionDataBase.dbFileName(accountName()));
    QFile::remove(roomSubscriptionDataBase.dbFileName(otherAccountName()));
}

void LocalRoomSubscriptionsDatabaseTest::shouldDefaultValues()
{
    const LocalRoomSubscriptionsDatabase roomSubscriptionDataBase;
    QCOMPARE(roomSubscriptionDataBase.schemaDatabaseStr(), u"CREATE TABLE ROOMSUBSCRIPTIONS (subscriptionId TEXT PRIMARY KEY NOT NULL, roomId TEXT)"_s);
}

void LocalRoomSubscriptionsDatabaseTest::shouldVerifyDbFileName()
{
    const LocalRoomSubscriptionsDatabase roomSubscriptionDataBase;
    QCOMPARE(roomSubscriptionDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/database/roomsubscriptions/myAccount/myAccount.sqlite"_s);
}

void LocalRoomSubscriptionsDatabaseTest::shouldStoreRoomSubscription()
{
    // GIVEN
    LocalRoomSubscriptionsDatabase logger;

    QByteArray subscriptionId = "sub1"_ba;
    QByteArray roomId = "foo1"_ba;
    logger.insertRoomSubscription(otherAccountName(), subscriptionId, roomId);

    QByteArray subscriptionId2 = "sub2"_ba;
    QByteArray roomId2 = "foo2"_ba;
    logger.insertRoomSubscription(otherAccountName(), subscriptionId2, roomId2);

    QByteArray subscriptionId3 = "sub3"_ba;
    QByteArray roomId3 = "foo3"_ba;
    logger.insertRoomSubscription(otherAccountName(), subscriptionId3, roomId3);

    // WHEN
    auto tableModel = logger.createRoomSubscriptionsModel(otherAccountName());

    // THEN
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 3);
    const QSqlRecord record0 = tableModel->record(0);
    QCOMPARE(record0.value(int(RoomSubscriptionFields::SubscriptionId)).toByteArray(), subscriptionId);
    QCOMPARE(record0.value(int(RoomSubscriptionFields::RoomId)).toByteArray(), roomId);

    const QSqlRecord record1 = tableModel->record(1);
    QCOMPARE(record1.value(int(RoomSubscriptionFields::SubscriptionId)).toByteArray(), subscriptionId2);
    QCOMPARE(record1.value(int(RoomSubscriptionFields::RoomId)).toByteArray(), roomId2);
}

void LocalRoomSubscriptionsDatabaseTest::shouldDeleteRoomSubscription() // this test depends on shouldStoreRoomPendingTypedInfo()
{
    // GIVEN
    LocalRoomSubscriptionsDatabase logger;
    const QByteArray subscriptionId = "sub1"_ba;

    // WHEN
    logger.deleteRoomSubscription(otherAccountName(), subscriptionId);

    // THEN
    auto tableModel = logger.createRoomSubscriptionsModel(otherAccountName());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 2);
}

void LocalRoomSubscriptionsDatabaseTest::shouldDeleteSubscriptionInvalidSubscriptionId() // this test depends on shouldStoreRoomPendingTypedInfo()
{
    // GIVEN
    LocalRoomSubscriptionsDatabase logger;
    const QByteArray subscriptionId = "foo2"_ba;

    // WHEN
    logger.deleteRoomSubscription(otherAccountName(), subscriptionId);

    // THEN
    auto tableModel = logger.createRoomSubscriptionsModel(otherAccountName());
    QVERIFY(tableModel);
    QCOMPARE(tableModel->rowCount(), 2);
}

void LocalRoomSubscriptionsDatabaseTest::shouldGetRoomId()
{
    LocalRoomSubscriptionsDatabase logger;

    const QByteArray subscriptionId2 = "sub2"_ba;
    QCOMPARE(logger.roomId(otherAccountName(), subscriptionId2), "foo2"_ba);

    // No existing subscription5
    const QByteArray subscriptionId5 = "sub5"_ba;
    QCOMPARE(logger.roomId(otherAccountName(), subscriptionId5), QByteArray{});
}
#include "moc_localroomsubscriptionsdatabasetest.cpp"

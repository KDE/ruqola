/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroomsdatabasetest.h"
#include "localdatabase/localroomsdatabase.h"
#include "room.h"
#include <QFile>
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(LocalRoomsDatabaseTest)
static QString accountName()
{
    return QStringLiteral("myAccount");
}
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
}

void LocalRoomsDatabaseTest::shouldDefaultValues()
{
    LocalRoomsDatabase roomDatabase;
    QCOMPARE(roomDatabase.schemaDatabaseStr(), QStringLiteral("CREATE TABLE ROOMS (roomId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)"));
}

void LocalRoomsDatabaseTest::shouldVerifyDbFileName()
{
    LocalRoomsDatabase accountDataBase;
    QCOMPARE(accountDataBase.dbFileName(accountName()),
             QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/database/rooms/myAccount/myAccount.sqlite"));
}

void LocalRoomsDatabaseTest::shouldStoreRoomsSettings()
{
    {
        LocalRoomsDatabase roomDataBase;
        QByteArray ba = "{}";
        // roomDataBase.addRoom(accountName(), ba); // TODO

        //        // WHEN
        QByteArray getInfo = roomDataBase.jsonRoom(accountName(), QString()); // TODO

        // THEN
        QCOMPARE(getInfo, ba);
    }
}

#include "moc_localroomsdatabasetest.cpp"

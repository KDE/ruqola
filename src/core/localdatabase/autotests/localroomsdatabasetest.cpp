/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroomsdatabasetest.h"
#include "localdatabase/localroomsdatabase.h"
#include "room.h"
#include "ruqola_autotest_helper.h"
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
        const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/rooms/direct-room.json");
        const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);
        LocalRoomsDatabase roomDataBase;
        Room r;
        r.parseSubscriptionRoom(fields);
        const QByteArray ba = Room::serialize(&r, false);
        roomDataBase.addRoom(accountName(), &r);

        //        // WHEN
        QByteArray getInfo = roomDataBase.jsonRoom(accountName(), r.roomId());

        // THEN
        QCOMPARE(getInfo, ba);
    }
}

#include "moc_localroomsdatabasetest.cpp"

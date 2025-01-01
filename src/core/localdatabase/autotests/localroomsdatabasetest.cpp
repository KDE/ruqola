/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroomsdatabasetest.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabase/localroomsdatabase.h"
#include "room.h"
#include "ruqola_autotest_helper.h"
#include <QFile>
#include <QStandardPaths>

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

#include "moc_localroomsdatabasetest.cpp"

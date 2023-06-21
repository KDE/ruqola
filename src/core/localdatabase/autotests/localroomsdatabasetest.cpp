/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroomsdatabasetest.h"
#include "localdatabase/localroomsdatabase.h"
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(LocalRoomsDatabaseTest)

LocalRoomsDatabaseTest::LocalRoomsDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalRoomsDatabaseTest::shouldDefaultValues()
{
    LocalRoomsDatabase logger;
    QCOMPARE(logger.schemaDatabaseStr(), QStringLiteral("CREATE TABLE ROOMS (roomId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)"));
}

#include "moc_localroomsdatabasetest.cpp"

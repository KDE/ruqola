/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabaseutilstest.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabase/localdatabaseutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(LocalDatabaseUtilsTest)

LocalDatabaseUtilsTest::LocalDatabaseUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void LocalDatabaseUtilsTest::shouldAdaptRoomName()
{
    QCOMPARE(LocalDatabaseUtils::fixRoomName(u"foo"_s), u"foo"_s);
    QCOMPARE(LocalDatabaseUtils::fixRoomName(u"1236"_s), u"1236"_s);
    QCOMPARE(LocalDatabaseUtils::fixRoomName(u"bla foo"_s), u"bla foo"_s);
    QCOMPARE(LocalDatabaseUtils::fixRoomName(u"bla/foo"_s), u"blafoo"_s);
    QCOMPARE(LocalDatabaseUtils::fixRoomName(u"bla:foo"_s), u"blafoo"_s);
}

void LocalDatabaseUtilsTest::shouldCheckPath()
{
    QCOMPARE(LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Messages), u"messages/"_s);
    QCOMPARE(LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Rooms), u"rooms/"_s);
    QCOMPARE(LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Accounts), u"account/"_s);
    QCOMPARE(LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Global), u"global/"_s);
}

void LocalDatabaseUtilsTest::shouldCheckDataBase()
{
    QCOMPARE(LocalDatabaseUtils::deleteMessage(), u"DELETE FROM MESSAGES WHERE messageId = ?"_s);
    QCOMPARE(LocalDatabaseUtils::insertReplaceMessage(), u"INSERT OR REPLACE INTO MESSAGES VALUES (?, ?, ?)"_s);
    QCOMPARE(LocalDatabaseUtils::deleteRoom(), u"DELETE FROM ROOMS WHERE roomId = ?"_s);
    QCOMPARE(LocalDatabaseUtils::insertReplaceRoom(), u"INSERT OR REPLACE INTO ROOMS VALUES (?, ?, ?)"_s);
    QCOMPARE(LocalDatabaseUtils::deleteAccount(), u"DELETE FROM ACCOUNT WHERE accountName = ?"_s);
    QCOMPARE(LocalDatabaseUtils::updateAccount(), u"INSERT OR REPLACE INTO ACCOUNT VALUES (?, ?)"_s);
    QCOMPARE(LocalDatabaseUtils::insertReplaceGlobal(), u"INSERT OR REPLACE INTO GLOBAL VALUES (?, ?)"_s);
    QCOMPARE(LocalDatabaseUtils::removeGlobal(), u"DELETE FROM GLOBAL WHERE identifier = ?"_s);
    QCOMPARE(LocalDatabaseUtils::timestampGlobal(), u"SELECT timestamp FROM GLOBAL WHERE identifier = \"%1\""_s);
    QCOMPARE(LocalDatabaseUtils::deleteMessageFromLogs(), u"DELETE FROM LOGS WHERE messageId = ?"_s);
    QCOMPARE(LocalDatabaseUtils::insertReplaceMessageFromLogs(), u"INSERT OR REPLACE INTO LOGS VALUES (?, ?, ?, ?)"_s);
    QCOMPARE(LocalDatabaseUtils::jsonAccount(), u"SELECT json FROM ACCOUNT WHERE accountName = \"%1\""_s);
}

#include "moc_localdatabaseutilstest.cpp"

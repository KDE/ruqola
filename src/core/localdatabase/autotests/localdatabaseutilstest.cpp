/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabaseutilstest.h"
#include "localdatabase/localdatabaseutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(LocalDatabaseUtilsTest)

LocalDatabaseUtilsTest::LocalDatabaseUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void LocalDatabaseUtilsTest::shouldAdaptRoomName()
{
    QCOMPARE(LocalDatabaseUtils::fixRoomName(QStringLiteral("foo")), QStringLiteral("foo"));
    QCOMPARE(LocalDatabaseUtils::fixRoomName(QStringLiteral("1236")), QStringLiteral("1236"));
    QCOMPARE(LocalDatabaseUtils::fixRoomName(QStringLiteral("bla foo")), QStringLiteral("bla foo"));
    QCOMPARE(LocalDatabaseUtils::fixRoomName(QStringLiteral("bla/foo")), QStringLiteral("blafoo"));
    QCOMPARE(LocalDatabaseUtils::fixRoomName(QStringLiteral("bla:foo")), QStringLiteral("blafoo"));
}

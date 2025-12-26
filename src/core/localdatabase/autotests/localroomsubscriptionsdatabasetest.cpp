/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "localroomsubscriptionsdatabasetest.h"
#include "localdatabase/localroomsubscriptionsdatabase.h"
#include <QTest>
QTEST_GUILESS_MAIN(LocalRoomSubscriptionsDatabaseTest)
using namespace Qt::Literals::StringLiterals;
LocalRoomSubscriptionsDatabaseTest::LocalRoomSubscriptionsDatabaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalRoomSubscriptionsDatabaseTest::shouldDefaultValues()
{
    LocalRoomSubscriptionsDatabase roomSubscriptionDataBase;
    QCOMPARE(roomSubscriptionDataBase.schemaDatabaseStr(), u"CREATE TABLE ROOMSUBSCRIPTION (subscriptionId TEXT PRIMARY KEY NOT NULL, roomId TEXT)"_s);
}

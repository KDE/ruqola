/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "banneduserstest.h"
#include "bannedusers/bannedusers.h"
#include <QTest>
QTEST_GUILESS_MAIN(BannedUsersTest)

BannedUsersTest::BannedUsersTest(QObject *parent)
    : QObject{parent}
{
}

void BannedUsersTest::shouldHaveDefaultValues()
{
    const BannedUsers w;
    QVERIFY(w.isEmpty());
    QVERIFY(w.bannedUsers().isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.total(), 0);
    QCOMPARE(w.bannedUsersCount(), 0);
}

#include "moc_banneduserstest.cpp"

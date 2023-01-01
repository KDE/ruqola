/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatustest.h"
#include "customusers/customuserstatus.h"
#include <QTest>
QTEST_GUILESS_MAIN(CustomUserStatusTest)

CustomUserStatusTest::CustomUserStatusTest(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusTest::shouldHaveDefaultValues()
{
    CustomUserStatus w;
    QVERIFY(!w.isValid());

    QVERIFY(w.name().isEmpty());
    QVERIFY(w.identifier().isEmpty());
    QCOMPARE(w.statusType(), User::PresenceStatus::Unknown);
    QCOMPARE(w.updatedAt(), -1);
}

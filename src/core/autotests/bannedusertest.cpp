/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannedusertest.h"
#include "bannedusers/banneduser.h"
#include <QTest>
QTEST_GUILESS_MAIN(BannedUserTest)

BannedUserTest::BannedUserTest(QObject *parent)
    : QObject{parent}
{
}

void BannedUserTest::shouldHaveDefaultValues()
{
    const BannedUser w;
    QVERIFY(w.userName().isEmpty());
    QVERIFY(w.name().isEmpty());
}

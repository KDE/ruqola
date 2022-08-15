/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmanagertest.h"
#include "licenses/licensesmanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(LicensesManagerTest)
LicensesManagerTest::LicensesManagerTest(QObject *parent)
    : QObject{parent}
{
}

void LicensesManagerTest::shouldHaveDefaultValues()
{
    LicensesManager w;
    QVERIFY(w.licenses().isEmpty());
    QVERIFY(!w.hasLicense(QStringLiteral("bla")));
}

/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolatest.h"
#include "ruqola.h"
#include <QTest>

QTEST_GUILESS_MAIN(RuqolaTest)

RuqolaTest::RuqolaTest(QObject *parent)
    : QObject(parent)
{
}

void RuqolaTest::shouldHaveDefaultValue()
{
    Ruqola r(nullptr);
    QVERIFY(r.accountManager());
}

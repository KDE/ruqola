/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolatest.h"
#include "ruqola.h"
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(RuqolaTest)

RuqolaTest::RuqolaTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RuqolaTest::shouldHaveDefaultValue()
{
    Ruqola r(nullptr);
    QVERIFY(r.accountManager());
}

#include "moc_ruqolatest.cpp"

/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autotranslatelanguagestest.h"
#include "autotranslate/autotranslatelanguages.h"
#include <QTest>
QTEST_GUILESS_MAIN(AutotranslateLanguagesTest)

AutotranslateLanguagesTest::AutotranslateLanguagesTest(QObject *parent)
    : QObject(parent)
{
}

void AutotranslateLanguagesTest::shouldHaveDefaultValues()
{
    AutotranslateLanguages w;
    QVERIFY(w.isEmpty());
    QCOMPARE(w.count(), 0);
}

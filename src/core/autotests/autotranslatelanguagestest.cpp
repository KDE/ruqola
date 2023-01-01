/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    QVERIFY(w.autotranslateLanguage().isEmpty());
}

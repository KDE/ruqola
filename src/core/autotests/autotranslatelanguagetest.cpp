/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autotranslatelanguagetest.h"
#include "autotranslate/autotranslatelanguage.h"
#include <QTest>
QTEST_GUILESS_MAIN(AutotranslateLanguageTest)

AutotranslateLanguageTest::AutotranslateLanguageTest(QObject *parent)
    : QObject(parent)
{
}

void AutotranslateLanguageTest::shouldHaveDefaultValue()
{
    AutotranslateLanguage w;
    QVERIFY(w.language().isEmpty());
    QVERIFY(w.displayLanguage().isEmpty());
}

/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unicodeemoticontest.h"
#include "emoticons/unicodeemoticon.h"
#include <QTest>
QTEST_GUILESS_MAIN(UnicodeEmoticonTest)

UnicodeEmoticonTest::UnicodeEmoticonTest(QObject *parent)
    : QObject(parent)
{
}

void UnicodeEmoticonTest::shouldHaveDefaultValue()
{
    UnicodeEmoticon w;
    QVERIFY(w.identifier().isEmpty());
    QVERIFY(w.category().isEmpty());
    QVERIFY(w.unicode().isEmpty());
    QVERIFY(w.aliases().isEmpty());
    QVERIFY(w.key().isEmpty());
    QVERIFY(!w.isValid());
}

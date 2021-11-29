/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

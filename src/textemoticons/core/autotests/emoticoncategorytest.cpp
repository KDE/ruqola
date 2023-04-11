/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticoncategorytest.h"
#include "emoticoncategory.h"
#include <QTest>
QTEST_GUILESS_MAIN(EmoticonCategoryTest)

EmoticonCategoryTest::EmoticonCategoryTest(QObject *parent)
    : QObject{parent}
{
}

void EmoticonCategoryTest::shouldHaveDefaultValues()
{
    TextEmoticonsCore::EmoticonCategory w;
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.category().isEmpty());
    QVERIFY(w.i18nName().isEmpty());
    QCOMPARE(w.order(), 20);
}

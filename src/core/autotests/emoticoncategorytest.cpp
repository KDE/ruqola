/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncategorytest.h"
#include "emoticons/emoticoncategory.h"
#include <QTest>
QTEST_GUILESS_MAIN(EmoticonCategoryTest)
EmoticonCategoryTest::EmoticonCategoryTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonCategoryTest::shouldHaveDefaultValues()
{
    EmoticonCategory cat;
    QVERIFY(cat.category().isEmpty());
    QVERIFY(cat.name().isEmpty());
}

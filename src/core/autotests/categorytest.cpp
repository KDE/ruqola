/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "categorytest.h"
#include "categories/category.h"
#include <QTest>

QTEST_GUILESS_MAIN(CategoryTest)

CategoryTest::CategoryTest(QObject *parent)
    : QObject{parent}
{
}

void CategoryTest::shouldHaveDefaultValues()
{
    const Category cat;
    QVERIFY(!cat.defaultCategory());
    QVERIFY(!cat.showUnreads());
    QVERIFY(cat.identifier().isEmpty());
    QVERIFY(cat.name().isEmpty());
    QVERIFY(!cat.isValid());
}

#include "moc_categorytest.cpp"

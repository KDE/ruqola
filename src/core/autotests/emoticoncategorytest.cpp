/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

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

void EmoticonCategoryTest::shouldAssignValues()
{
    EmoticonCategory cat;
    const QString categoryName{QStringLiteral("cat")};
    cat.setCategory(categoryName);
    QCOMPARE(cat.category(), categoryName);

    const QString name{QStringLiteral("name")};
    cat.setName(name);
    QCOMPARE(cat.name(), name);

    const QString namei18n{QStringLiteral("i18n")};
    cat.setI18nName(namei18n);
    QCOMPARE(cat.i18nName(), namei18n);
}

#include "moc_emoticoncategorytest.cpp"

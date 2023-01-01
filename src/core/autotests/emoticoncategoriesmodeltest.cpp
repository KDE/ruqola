/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncategoriesmodeltest.h"
#include "model/emoticoncategoriesmodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(EmoticonCategoriesModelTest)
EmoticonCategoriesModelTest::EmoticonCategoriesModelTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonCategoriesModelTest::shouldHaveDefaultValues()
{
    EmoticonCategoriesModel w;
    // TODO
}

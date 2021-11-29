/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

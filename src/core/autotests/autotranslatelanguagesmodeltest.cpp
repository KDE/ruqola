/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autotranslatelanguagesmodeltest.h"
#include "model/autotranslatelanguagesmodel.h"
#include "test_model_helpers.h"
#include <QTest>
QTEST_GUILESS_MAIN(AutotranslateLanguagesModelTest)

AutotranslateLanguagesModelTest::AutotranslateLanguagesModelTest(QObject *parent)
    : QObject(parent)
{
}

void AutotranslateLanguagesModelTest::shouldHaveDefaultValues()
{
    AutotranslateLanguagesModel model;
    // TODO
}

#include "moc_autotranslatelanguagesmodeltest.cpp"

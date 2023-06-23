/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importdatawizardtest.h"
#include "importexportdata/importdata/importdatawizard.h"
#include <QTest>

QTEST_MAIN(ImportDataWizardTest)
ImportDataWizardTest::ImportDataWizardTest(QObject *parent)
    : QObject{parent}
{
}

void ImportDataWizardTest::shouldHaveDefaultValues()
{
    ImportDataWizard w;
    // TODO
}

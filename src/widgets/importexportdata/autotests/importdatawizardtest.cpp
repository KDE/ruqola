/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importdatawizardtest.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportdata/importdata/importdatafinishpage.h"
#include "importexportdata/importdata/importdataselectaccountpage.h"
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

    auto mImportDataSelectAccountPage = w.findChild<ImportDataSelectAccountPage *>(u"mImportDataSelectAccountPage"_s);
    QVERIFY(mImportDataSelectAccountPage);

    auto mImportDataFinishPage = w.findChild<ImportDataFinishPage *>(u"mImportDataSelectAccountPage"_s);
    QVERIFY(mImportDataFinishPage);
}

#include "moc_importdatawizardtest.cpp"

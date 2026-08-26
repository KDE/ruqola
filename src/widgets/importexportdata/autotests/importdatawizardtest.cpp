/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importdatawizardtest.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportdata/importdata/importdatafinishpage.h"
#include "importexportdata/importdata/importdataselectaccountpage.h"
#include "importexportdata/importdata/importdatawizard.h"
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(ImportDataWizardTest)
ImportDataWizardTest::ImportDataWizardTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ImportDataWizardTest::shouldHaveDefaultValues()
{
    const ImportDataWizard w;

    auto mImportDataSelectAccountPage = w.findChild<ImportDataSelectAccountPage *>(u"mImportDataSelectAccountPage"_s);
    QVERIFY(mImportDataSelectAccountPage);

    auto mImportDataFinishPage = w.findChild<ImportDataFinishPage *>(u"mImportDataFinishPage"_s);
    QVERIFY(mImportDataFinishPage);
}

#include "moc_importdatawizardtest.cpp"

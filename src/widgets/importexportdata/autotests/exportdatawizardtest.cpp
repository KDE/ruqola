/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatawizardtest.h"
#include "importexportdata/exportdata/exportdatawizard.h"
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(ExportDataWizardTest)

ExportDataWizardTest::ExportDataWizardTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExportDataWizardTest::shouldHaveDefaultValues()
{
    ExportDataWizard w;
    // TODO
}

#include "moc_exportdatawizardtest.cpp"

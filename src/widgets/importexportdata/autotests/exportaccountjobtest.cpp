/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportaccountjobtest.h"
#include "importexportdata/exportdata/exportaccountjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(ExportAccountJobTest)
ExportAccountJobTest::ExportAccountJobTest(QObject *parent)
    : QObject{parent}
{
}

void ExportAccountJobTest::shouldHaveDefaultValues()
{
    ExportAccountJob job({});
    QVERIFY(!job.canStart());
    QVERIFY(job.listAccounts().isEmpty());
}

#include "moc_exportaccountjobtest.cpp"

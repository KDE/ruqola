/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exportdataselectaccountpagetest.h"
#include "importexportdata/exportdataselectaccountpage.h"
#include <QTest>

QTEST_MAIN(ExportDataSelectAccountPageTest)
ExportDataSelectAccountPageTest::ExportDataSelectAccountPageTest(QObject *parent)
    : QObject{parent}
{
}

void ExportDataSelectAccountPageTest::shouldHaveDefaultValues()
{
    ExportDataSelectAccountPage w;
    // TODO
}

#include "moc_exportdataselectaccountpagetest.cpp"

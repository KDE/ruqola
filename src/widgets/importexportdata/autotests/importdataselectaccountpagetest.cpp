/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdataselectaccountpagetest.h"
#include "importexportdata/importdata/importdataselectaccountpage.h"
#include <QTest>

QTEST_MAIN(ImportDataSelectAccountPageTest)
ImportDataSelectAccountPageTest::ImportDataSelectAccountPageTest(QObject *parent)
    : QObject{parent}
{
}

void ImportDataSelectAccountPageTest::shouldHaveDefaultValues()
{
    ImportDataSelectAccountPage w;
    // TODO
}

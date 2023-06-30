/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdataselectaccountpagetest.h"
#include "importexportdata/importdata/importdataselectaccountpage.h"
#include <KUrlRequester>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ImportDataSelectAccountPageTest)
ImportDataSelectAccountPageTest::ImportDataSelectAccountPageTest(QObject *parent)
    : QObject{parent}
{
}

void ImportDataSelectAccountPageTest::shouldHaveDefaultValues()
{
    ImportDataSelectAccountPage w;

    auto mUrlRequester = w.findChild<KUrlRequester *>(QStringLiteral("mUrlRequester"));
    QVERIFY(mUrlRequester);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
}

#include "moc_importdataselectaccountpagetest.cpp"

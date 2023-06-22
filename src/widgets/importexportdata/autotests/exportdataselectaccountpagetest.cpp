/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exportdataselectaccountpagetest.h"
#include "importexportdata/exportdataselectaccountlistwidget.h"
#include "importexportdata/exportdataselectaccountpage.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ExportDataSelectAccountPageTest)
ExportDataSelectAccountPageTest::ExportDataSelectAccountPageTest(QObject *parent)
    : QObject{parent}
{
}

void ExportDataSelectAccountPageTest::shouldHaveDefaultValues()
{
    ExportDataSelectAccountPage w;

    auto mExportDataSelectAccountListWidget = w.findChild<ExportDataSelectAccountListWidget *>(QStringLiteral("mExportDataSelectAccountListWidget"));
    QVERIFY(mExportDataSelectAccountListWidget);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}

#include "moc_exportdataselectaccountpagetest.cpp"

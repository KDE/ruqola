/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exportdataselectaccountpagetest.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportdata/exportdata/exportdataselectaccountlistwidget.h"
#include "importexportdata/exportdata/exportdataselectaccountpage.h"
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

    auto mExportDataSelectAccountListWidget = w.findChild<ExportDataSelectAccountListWidget *>(u"mExportDataSelectAccountListWidget"_s);
    QVERIFY(mExportDataSelectAccountListWidget);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}

#include "moc_exportdataselectaccountpagetest.cpp"

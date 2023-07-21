/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountlistwidgettest.h"
#include "importexportdata/exportdata/exportdataselectaccountlistwidget.h"

#include <QTest>

QTEST_MAIN(ExportDataSelectAccountListWidgetTest)
ExportDataSelectAccountListWidgetTest::ExportDataSelectAccountListWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ExportDataSelectAccountListWidgetTest::shouldHaveDefaultValues()
{
    ExportDataSelectAccountListWidget w;
    // TODO
}

#include "moc_exportdataselectaccountlistwidgettest.cpp"

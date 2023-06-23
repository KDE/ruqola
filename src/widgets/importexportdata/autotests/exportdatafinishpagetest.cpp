/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpagetest.h"
#include "importexportdata/exportdata/exportdatafinishpage.h"
#include <QTest>

QTEST_MAIN(ExportDataFinishPageTest)
ExportDataFinishPageTest::ExportDataFinishPageTest(QObject *parent)
    : QObject{parent}
{
}

void ExportDataFinishPageTest::shouldHaveDefaultValues()
{
    ExportDataFinishPage w;
    // TODO
}

#include "moc_exportdatafinishpagetest.cpp"

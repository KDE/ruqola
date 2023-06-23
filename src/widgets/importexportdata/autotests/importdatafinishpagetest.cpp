/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatafinishpagetest.h"
#include "importexportdata/importdata/importdatafinishpage.h"
#include <QTest>
QTEST_MAIN(ImportDataFinishPageTest)

ImportDataFinishPageTest::ImportDataFinishPageTest(QObject *parent)
    : QObject{parent}
{
}

void ImportDataFinishPageTest::shouldHaveDefaultValues()
{
    ImportDataFinishPage w;
    // TODO
}

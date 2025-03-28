/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "importaccountjobtest.h"
#include "importexportdata/importdata/importaccountjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(ImportAccountJobTest)

ImportAccountJobTest::ImportAccountJobTest(QObject *parent)
    : QObject{parent}
{
}

void ImportAccountJobTest::shouldHaveDefaultValues()
{
    ImportAccountJob w({});

    // TODO
}

#include "moc_importaccountjobtest.cpp"

/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importexportutilstest.h"
#include "importexportdata/importexportutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(ImportExportUtilsTest)
ImportExportUtilsTest::ImportExportUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ImportExportUtilsTest::shouldReturnValues()
{
    QCOMPARE(ImportExportUtils::configPath(), QStringLiteral("config"));
    QCOMPARE(ImportExportUtils::localPath(), QStringLiteral("local"));
    QCOMPARE(ImportExportUtils::cachePath(), QStringLiteral("cache"));
    QCOMPARE(ImportExportUtils::logsPath(), QStringLiteral("logs"));
    QCOMPARE(ImportExportUtils::databasePath(), QStringLiteral("database"));
}

#include "moc_importexportutilstest.cpp"

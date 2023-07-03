/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importexportutilsjob.h"
#include "importexportdata/importexportutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(ImportExportUtilsJob)
ImportExportUtilsJob::ImportExportUtilsJob(QObject *parent)
    : QObject{parent}
{
}

void ImportExportUtilsJob::shouldReturnValues()
{
    QCOMPARE(ImportExportUtils::configPath(), QStringLiteral("config"));
    QCOMPARE(ImportExportUtils::localPath(), QStringLiteral("local"));
    QCOMPARE(ImportExportUtils::logsPath(), QStringLiteral("cache"));
    QCOMPARE(ImportExportUtils::databasePath(), QStringLiteral("logs"));
    QCOMPARE(ImportExportUtils::cachePath(), QStringLiteral("database"));
}

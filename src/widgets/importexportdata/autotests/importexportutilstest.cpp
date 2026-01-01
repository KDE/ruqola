/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importexportutilstest.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportdata/importexportutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(ImportExportUtilsTest)
ImportExportUtilsTest::ImportExportUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ImportExportUtilsTest::shouldReturnValues()
{
    QCOMPARE(ImportExportUtils::configPath(), u"config"_s);
    QCOMPARE(ImportExportUtils::localPath(), u"local"_s);
    QCOMPARE(ImportExportUtils::cachePath(), u"cache"_s);
    QCOMPARE(ImportExportUtils::logsPath(), u"logs"_s);
    QCOMPARE(ImportExportUtils::databasePath(), u"database"_s);
}

#include "moc_importexportutilstest.cpp"

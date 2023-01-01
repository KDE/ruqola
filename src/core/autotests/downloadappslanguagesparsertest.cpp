/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "downloadappslanguagesparsertest.h"
#include "downloadappslanguages/downloadappslanguagesparser.h"
#include <QTest>
QTEST_GUILESS_MAIN(DownloadAppsLanguagesParserTest)
DownloadAppsLanguagesParserTest::DownloadAppsLanguagesParserTest(QObject *parent)
    : QObject(parent)
{
}

void DownloadAppsLanguagesParserTest::shouldParseFile_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("identifier");
    QTest::addRow("empty") << QStringLiteral("empty") << 0;
    QTest::addRow("languages") << QStringLiteral("languages") << 23;
}

void DownloadAppsLanguagesParserTest::shouldParseFile()
{
    QFETCH(QString, filename);
    QFETCH(int, identifier);
    DownloadAppsLanguagesParser parser;
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/downloadappslanguages/") + filename;
    parser.setFilename(originalJsonFile);
    QVERIFY(parser.parse());
    QCOMPARE(parser.map().count(), identifier);
}

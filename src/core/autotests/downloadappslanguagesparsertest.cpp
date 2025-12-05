/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "downloadappslanguagesparsertest.h"

#include "downloadappslanguages/downloadappslanguagesparser.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(DownloadAppsLanguagesParserTest)
DownloadAppsLanguagesParserTest::DownloadAppsLanguagesParserTest(QObject *parent)
    : QObject(parent)
{
}

void DownloadAppsLanguagesParserTest::shouldParseFile_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("identifier");
    QTest::addRow("empty") << u"empty"_s << 0;
    QTest::addRow("languages") << u"languages"_s << 23;
}

void DownloadAppsLanguagesParserTest::shouldParseFile()
{
    QFETCH(QString, filename);
    QFETCH(int, identifier);
    DownloadAppsLanguagesParser parser;
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/downloadappslanguages/"_L1 + filename;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    auto result = parser.parse(obj);
    QCOMPARE(result.count(), identifier);
}

#include "moc_downloadappslanguagesparsertest.cpp"

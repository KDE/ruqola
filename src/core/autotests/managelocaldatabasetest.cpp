/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managelocaldatabasetest.h"
#include "managelocaldatabase.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ManageLocalDatabaseTest)
ManageLocalDatabaseTest::ManageLocalDatabaseTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ManageLocalDatabaseTest::shouldHaveDefaultValues()
{
    const ManageLocalDatabase d(nullptr);
    // TODO
}

void ManageLocalDatabaseTest::shouldParseHistoryCursor_data()
{
    QTest::addColumn<QByteArray>("json");
    QTest::addColumn<qint64>("cursor");

    QTest::addRow("empty") << "{}"_ba << ManageLocalDatabase::HistoryCursorUnknown;
    QTest::addRow("no-previous") << R"({"cursor":{"next":null}})"_ba << ManageLocalDatabase::HistoryCursorUnknown;
    QTest::addRow("previous-null") << R"({"cursor":{"next":null,"previous":null}})"_ba << ManageLocalDatabase::HistoryCursorEnd;
    QTest::addRow("previous") << R"({"cursor":{"next":null,"previous":"1762425600123"}})"_ba << qint64(1762425600123);
    QTest::addRow("previous-not-timestamp") << R"({"cursor":{"previous":"abc"}})"_ba << ManageLocalDatabase::HistoryCursorUnknown;
    QTest::addRow("previous-zero") << R"({"cursor":{"previous":"0"}})"_ba << ManageLocalDatabase::HistoryCursorUnknown;
}

void ManageLocalDatabaseTest::shouldParseHistoryCursor()
{
    QFETCH(QByteArray, json);
    QFETCH(qint64, cursor);
    QCOMPARE(ManageLocalDatabase::parseHistoryCursor(QJsonDocument::fromJson(json).object()), cursor);
}

#include "moc_managelocaldatabasetest.cpp"

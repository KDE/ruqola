/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsemessageurlutilstest.h"
#include "parsemessageurlutils.h"
#include <QTest>
QTEST_MAIN(ParseMessageUrlUtilsTest)
ParseMessageUrlUtilsTest::ParseMessageUrlUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ParseMessageUrlUtilsTest::shouldHaveDefaultValues()
{
    ParseMessageUrlUtils w;
    QVERIFY(w.messageId().isEmpty());
    QVERIFY(w.roomId().isEmpty());
    QVERIFY(w.serverPath().isEmpty());
}

void ParseMessageUrlUtilsTest::shouldParseUrl_data()
{
    QTest::addColumn<QString>("messageUrl");
    QTest::addColumn<bool>("parsingValid");
    QTest::addRow("empty") << QString() << false;
    QTest::addRow("kde") << QStringLiteral("http://www.kde.org") << false;
    QTest::addRow("kde-1") << QStringLiteral("https://www.kde.org") << false;
    QTest::addRow("kde-2") << QStringLiteral("www.kde.org") << false;
}

void ParseMessageUrlUtilsTest::shouldParseUrl()
{
    QFETCH(QString, messageUrl);
    QFETCH(bool, parsingValid);
    ParseMessageUrlUtils w;
    QCOMPARE(w.parseUrl(messageUrl), parsingValid);
}

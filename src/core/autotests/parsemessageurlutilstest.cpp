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
    QVERIFY(w.serverHost().isEmpty());
    QVERIFY(w.path().isEmpty());
}

void ParseMessageUrlUtilsTest::shouldParseUrl_data()
{
    QTest::addColumn<QString>("messageUrl");
    QTest::addColumn<bool>("parsingValid");
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<QString>("roomId");
    QTest::addColumn<QString>("serverHost");
    QTest::addColumn<QString>("path");

    QTest::addRow("empty") << QString() << false << QString() << QString() << QString() << QString();
    QTest::addRow("kde") << QStringLiteral("http://www.kde.org") << false << QString() << QString() << QString() << QString();
    QTest::addRow("kde-1") << QStringLiteral("https://www.kde.org") << false << QString() << QString() << QString() << QString();
    QTest::addRow("kde-2") << QStringLiteral("www.kde.org") << false << QString() << QString() << QString() << QString();
    QTest::addRow("gorocketchat") << QStringLiteral("https://go.rocket.chat/") << true << QString() << QString() << QString() << QString();
    QTest::addRow("gorocketchat-1") << QStringLiteral(
        "https://go.rocket.chat/room?rid=NCrToCewka5MgMcDM&mid=Xope7b8WYWz82yHaq&host=www.kde.org&path=channel%2Ffoo%3Fmsg%3DXope7b8WYWz82yHaq")
                                    << true << QStringLiteral("Xope7b8WYWz82yHaq") << QStringLiteral("NCrToCewka5MgMcDM") << QStringLiteral("www.kde.org")
                                    << QStringLiteral("channel/foo?msg=Xope7b8WYWz82yHaq");
}

void ParseMessageUrlUtilsTest::shouldParseUrl()
{
    QFETCH(QString, messageUrl);
    QFETCH(bool, parsingValid);
    QFETCH(QString, messageId);
    QFETCH(QString, roomId);
    QFETCH(QString, serverHost);
    QFETCH(QString, path);
    ParseMessageUrlUtils w;
    QCOMPARE(w.parseUrl(messageUrl), parsingValid);
    QCOMPARE(w.messageId(), messageId);
    QCOMPARE(w.roomId(), roomId);
    QCOMPARE(w.serverHost(), serverHost);
    QCOMPARE(w.path(), path);
}

/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "utilstest.h"
#include "utils.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
QTEST_MAIN(UtilsTest)

UtilsTest::UtilsTest(QObject *parent)
    : QObject(parent)
{
}

void UtilsTest::shouldGenerateServerUrl_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QUrl>("output");
    QTest::newRow("empty") << QString() << QUrl();
    QTest::newRow("http") << QStringLiteral("http://foo.kde.org/") << QUrl(QStringLiteral("ws://foo.kde.org//websocket"));
    QTest::newRow("http2") << QStringLiteral("http://foo.kde.org") << QUrl(QStringLiteral("ws://foo.kde.org/websocket"));
    QTest::newRow("https") << QStringLiteral("https://foo.kde.org") << QUrl(QStringLiteral("wss://foo.kde.org/websocket"));
    QTest::newRow("withoutscheme") << QStringLiteral("foo.kde.org") << QUrl(QStringLiteral("wss://foo.kde.org/websocket"));
}

void UtilsTest::shouldGenerateServerUrl()
{
    QFETCH(QString, input);
    QFETCH(QUrl, output);
    QCOMPARE(Utils::generateServerUrl(input), output);
}

void UtilsTest::shouldMarkdownToRichText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("testurl") << QStringLiteral("http://www.kde.org http://www.kde.org")
                             << QStringLiteral("<a href=\"http://www.kde.org\">http://www.kde.org</a> <a href=\"http://www.kde.org\">http://www.kde.org</a>");
    QTest::newRow("bold") << QStringLiteral("*bla*")
                          << QStringLiteral("<b>bla</b>");
    QTest::newRow("multi star") << QStringLiteral("**bla**")
                                << QStringLiteral("<b>bla</b>");
    QTest::newRow("multi star2") << QStringLiteral("***bla***")
                                 << QStringLiteral("***bla***");
    QTest::newRow("multi star3") << QStringLiteral("***bla ******")
                                 << QStringLiteral("***bla ******");
    QTest::newRow("Remove <br/>") << QStringLiteral("foo<br />") << QStringLiteral("foo");
}

void UtilsTest::shouldMarkdownToRichText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(Utils::markdownToRichText(input), output);
}

void UtilsTest::shouldExtractRoomUserFromUrl_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();

    QTest::newRow("extractuser") << QStringLiteral("ruqola:/user/foo") << QStringLiteral("foo");
    QTest::newRow("extractroom") << QStringLiteral("ruqola:/room/foo") << QStringLiteral("foo");
    QTest::newRow("extractuser") << QStringLiteral("ruqola:/user2/foo") << QStringLiteral("ruqola:/user2/foo");
}

void UtilsTest::shouldExtractRoomUserFromUrl()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(Utils::extractRoomUserFromUrl(input), output);
}

void UtilsTest::shouldExtractGenerateRichText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("word@") << QStringLiteral("@foo") << QStringLiteral("<a href='ruqola:/user/foo'>@foo</a>");
    QTest::newRow("word@-2") << QStringLiteral("@foo.bla") << QStringLiteral("<a href='ruqola:/user/foo.bla'>@foo.bla</a>");
    QTest::newRow("word@-3") << QStringLiteral("@foo.bla.bli") << QStringLiteral("<a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a>");
    QTest::newRow("word@-4") << QStringLiteral("@foo.bla.bli dd") << QStringLiteral("<a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a> dd");
    QTest::newRow("word@-5") << QStringLiteral("bla bla 21 @foo.bla.bli dd") << QStringLiteral("bla bla 21 <a href='ruqola:/user/foo.bla.bli'>@foo.bla.bli</a> dd");

    QTest::newRow("word#") << QStringLiteral("#foo") << QStringLiteral("<a href='ruqola:/room/foo'>#foo</a>");
    QTest::newRow("word#-2") << QStringLiteral("#foo.bla") << QStringLiteral("<a href='ruqola:/room/foo.bla'>#foo.bla</a>");
    QTest::newRow("word#-3") << QStringLiteral("#foo.bla.bli") << QStringLiteral("<a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a>");
    QTest::newRow("word#-4") << QStringLiteral("#foo.bla.bli dd") << QStringLiteral("<a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd");
    QTest::newRow("word#-5") << QStringLiteral("bla bla 21 #foo.bla.bli dd") << QStringLiteral("bla bla 21 <a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd");
}

void UtilsTest::shouldExtractGenerateRichText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(Utils::generateRichText(input, {}), output);
}

void UtilsTest::shouldParseNotification_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("message");
    QTest::addColumn<QString>("sender");
    QTest::newRow("notification1") << QStringLiteral("notification") << QStringLiteral("title") << QStringLiteral("message") << QStringLiteral("sender");
}

void UtilsTest::shouldParseNotification()
{
    QFETCH(QString, fileName);
    QFETCH(QString, title);
    QFETCH(QString, message);
    QFETCH(QString, sender);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + fileName + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    const QJsonArray contents = obj.value(QLatin1String("args")).toArray();

    QString parseTitle;
    QString parseMessage;
    QString parseSender;

    Utils::parseNotification(contents, parseMessage, parseTitle, parseSender);
    QCOMPARE(parseMessage, message);
    QCOMPARE(parseTitle, title);
    QCOMPARE(parseSender, sender);
}

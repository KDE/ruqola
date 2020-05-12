/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
#include <KColorScheme>

QTEST_GUILESS_MAIN(UtilsTest)

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
    QTest::newRow("named-url") << QStringLiteral("[example.com](http://example.com)")
                               << QStringLiteral("<a href=\"http://example.com\">example.com</a>");
    QTest::newRow("bold") << QStringLiteral("*bla*")
                          << QStringLiteral("<b>bla</b>");
    QTest::newRow("multi star") << QStringLiteral("**bla**")
                                << QStringLiteral("<b>bla</b>");
    QTest::newRow("multi star2") << QStringLiteral("***bla***")
                                 << QStringLiteral("***bla***");
    QTest::newRow("multi star3") << QStringLiteral("***bla ******")
                                 << QStringLiteral("***bla ******");
    QTest::newRow("Remove <br/>") << QStringLiteral("foo<br />") << QStringLiteral("foo");

    QTest::newRow("0.6.3") << QStringLiteral("0.6.3") << QStringLiteral("0.6.3");
    //Bug 391520
    QTest::newRow("multi-line") << QStringLiteral("These are the options:\n- a\n- b") << QStringLiteral("These are the options:<br />\n- a<br />\n- b");
}

void UtilsTest::shouldMarkdownToRichText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QEXPECT_FAIL("bold", "Bug in kf5", Continue);
    QEXPECT_FAIL("multi star", "Bug in kf5", Continue);
    QEXPECT_FAIL("Remove <br/>", "Bug in kf5", Continue);
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
    QTest::newRow("word@-6") << QStringLiteral("@foo-bla") << QStringLiteral("<a href='ruqola:/user/foo-bla'>@foo-bla</a>");
    QTest::newRow("word@-7") << QStringLiteral("@foo_bla") << QStringLiteral("<a href='ruqola:/user/foo_bla'>@foo_bla</a>");
    QTest::newRow("word@-8") << QStringLiteral("bli@foo_bla") << QStringLiteral("bli@foo_bla");

    QTest::newRow("word#") << QStringLiteral("#foo") << QStringLiteral("<a href='ruqola:/room/foo'>#foo</a>");
    QTest::newRow("word#-2") << QStringLiteral("#foo.bla") << QStringLiteral("<a href='ruqola:/room/foo.bla'>#foo.bla</a>");
    QTest::newRow("word#-3") << QStringLiteral("#foo.bla.bli") << QStringLiteral("<a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a>");
    QTest::newRow("word#-4") << QStringLiteral("#foo.bla.bli dd") << QStringLiteral("<a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd");
    QTest::newRow("word#-5") << QStringLiteral("bla bla 21 #foo.bla.bli dd") << QStringLiteral("bla bla 21 <a href='ruqola:/room/foo.bla.bli'>#foo.bla.bli</a> dd");
    QTest::newRow("word#-6") << QStringLiteral("#foo-bla") << QStringLiteral("<a href='ruqola:/room/foo-bla'>#foo-bla</a>");
    QTest::newRow("word#-7") << QStringLiteral("#foo_bla") << QStringLiteral("<a href='ruqola:/room/foo_bla'>#foo_bla</a>");
    //Test parsing when it's in an url... don't replace it.

    QTest::newRow("url") << QStringLiteral("http://www.kde.org#foo_bla") << QStringLiteral("<a href=\"http://www.kde.org#foo_bla\">http://www.kde.org#foo_bla</a>");

    QTest::newRow("0.6.3") << QStringLiteral("0.6.3") << QStringLiteral("0.6.3");
    QTest::newRow("`code`") << QStringLiteral("foo `bla` bar `asdf` test") << QStringLiteral("foo <code>`bla`</code> bar <code>`asdf`</code> test");
}

void UtilsTest::shouldExtractGenerateRichText()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(Utils::generateRichText(input, QString()), output);
}

void UtilsTest::shouldHighlightText_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("username");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString() << QString();
    QTest::newRow("word@1") << QStringLiteral("@foo") << QString() << QStringLiteral("<a href='ruqola:/user/foo'>@foo</a>");
    KColorScheme colorScheme;
    const auto userMentionForegroundColor = colorScheme.foreground(KColorScheme::ActiveText).color().name();
    const auto userMentionBackgroundColor = colorScheme.background(KColorScheme::ActiveBackground).color().name();
    QTest::newRow("word@1-username") << QStringLiteral("@foo")
                                     << QStringLiteral("foo")
                                     << QStringLiteral("<a href='ruqola:/user/foo' style=\"color:%1;background-color:%2;\">@foo</a>")
        .arg(userMentionForegroundColor, userMentionBackgroundColor);
    QTest::newRow("word@2-username") << QStringLiteral("bla bla @foo")
                                     << QStringLiteral("foo")
                                     << QStringLiteral("bla bla <a href='ruqola:/user/foo' style=\"color:%1;background-color:%2;\">@foo</a>")
        .arg(userMentionForegroundColor, userMentionBackgroundColor);
}

void UtilsTest::shouldHighlightText()
{
    QFETCH(QString, input);
    QFETCH(QString, username);
    QFETCH(QString, output);
    QCOMPARE(Utils::generateRichText(input, username), output);
}

void UtilsTest::shouldParseNotification_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("message");
    QTest::addColumn<QString>("sender");
    QTest::addColumn<QString>("roomName");
    QTest::addColumn<QString>("channelType");
    QTest::addColumn<QString>("senderName");
    QTest::addColumn<QString>("senderUserName");
    QTest::newRow("notification1") << QStringLiteral("notification")
                                   << QStringLiteral("title")
                                   << QStringLiteral("pong")
                                   << QStringLiteral("tgrk5CZKgYGiSSqXp")
                                   << QString()
                                   << QStringLiteral("d")
                                   << QStringLiteral("Laurent")
                                   << QStringLiteral("laurent");
    //TODO
    QTest::newRow("notificationencrypted") << QStringLiteral("notificationencrypted")
                                           << QStringLiteral("title")
                                           << QStringLiteral("pong")
                                           << QStringLiteral("tgrk5CZKgYGiSSqXp")
                                           << QString()
                                           << QStringLiteral("d")
                                           << QStringLiteral("foo")
                                           << QStringLiteral("bla");

    QTest::newRow("notification2") << QStringLiteral("notification1")
                                   << QStringLiteral("my title")
                                   << QStringLiteral("@here")
                                   << QStringLiteral("Gsvg6BGoBfmPLoFie")
                                   << QStringLiteral("roomname example")
                                   << QStringLiteral("c")
                                   << QStringLiteral("foo")
                                   << QStringLiteral("foo.bla");
}

void UtilsTest::shouldParseNotification()
{
    QFETCH(QString, fileName);
    QFETCH(QString, title);
    QFETCH(QString, message);
    QFETCH(QString, sender);
    QFETCH(QString, roomName);
    QFETCH(QString, channelType);
    QFETCH(QString, senderName);
    QFETCH(QString, senderUserName);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + fileName + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object().value(QLatin1String("fields")).toObject();
    const QJsonArray contents = fields.value(QLatin1String("args")).toArray();

    const Utils::NotificationInfo info = Utils::parseNotification(contents);
    QEXPECT_FAIL("notificationencrypted", "Encrypted message not supported yet", Continue);
    QCOMPARE(info.message, message);
    QCOMPARE(info.title, title);
    QEXPECT_FAIL("notificationencrypted", "Encrypted message not supported yet", Continue);
    QCOMPARE(info.senderId, sender);
    QCOMPARE(info.roomName, roomName);
    QCOMPARE(info.channelType, channelType);
    QCOMPARE(info.senderUserName, senderUserName);
    QCOMPARE(info.senderName, senderName);
    QVERIFY(info.isValid());
}

void UtilsTest::shouldConvertTextWithUrl_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("convertedText");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("onlytext") << QStringLiteral("foo bla bli") << QStringLiteral("foo bla bli");
    QTest::newRow("test1") << QStringLiteral("[blo](http://www.kde.org)") << QStringLiteral("<a href='http://www.kde.org'>blo</a>");
    QTest::newRow("test2") << QStringLiteral("[](http://www.kde.org)") << QStringLiteral("(http://www.kde.org)");
    QTest::newRow("test3") << QStringLiteral("bla bla [blo](http://www.kde.org)") << QStringLiteral("bla bla <a href='http://www.kde.org'>blo</a>");
    QTest::newRow("test4") << QStringLiteral("bla bla [blo](http://www.kde.org) bli [blu](http://www.kdi.org)") << QStringLiteral("bla bla <a href='http://www.kde.org'>blo</a> bli <a href='http://www.kdi.org'>blu</a>");
    QTest::newRow("test5") << QStringLiteral("bla bla [blo]") << QStringLiteral("bla bla [blo]");
    QTest::newRow("test6") << QStringLiteral("bla bla [blo] bli") << QStringLiteral("bla bla [blo] bli");

    //Test <https://www.kde.org|bla>
    //QTest::newRow("https://www.kde.org|bla") << QStringLiteral("<https://www.kde.org|bla>") << QStringLiteral("<a href='https://www.kde.org'>bla</a>");
    //QTest::newRow("https://www.kde.org|bla 2 ") << QStringLiteral("bli <https://www.kde.org|bla> blu") << QStringLiteral("bli <a href='https://www.kde.org'>bla</a> blu");
}

void UtilsTest::shouldConvertTextWithUrl()
{
    QFETCH(QString, text);
    QFETCH(QString, convertedText);
    QCOMPARE(Utils::convertTextWithUrl(text), convertedText);
}

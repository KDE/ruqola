/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "utilstest.h"
#include "utils.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

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

void UtilsTest::shouldConvertTextWithUrl_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("convertedText");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("onlytext") << QStringLiteral("foo bla bli") << QStringLiteral("foo bla bli");
    QTest::newRow("test1") << QStringLiteral("[blo](http://www.kde.org)") << QStringLiteral("<a href='http://www.kde.org'>blo</a>");
    QTest::newRow("test2") << QStringLiteral("[](http://www.kde.org)") << QStringLiteral("(http://www.kde.org)");
    QTest::newRow("test3") << QStringLiteral("bla bla [blo](http://www.kde.org)") << QStringLiteral("bla bla <a href='http://www.kde.org'>blo</a>");
    QTest::newRow("test4") << QStringLiteral("bla bla [blo](http://www.kde.org) bli [blu](http://www.kdi.org)")
                           << QStringLiteral("bla bla <a href='http://www.kde.org'>blo</a> bli <a href='http://www.kdi.org'>blu</a>");
    QTest::newRow("test5") << QStringLiteral("bla bla [blo]") << QStringLiteral("bla bla [blo]");
    QTest::newRow("test6") << QStringLiteral("bla bla [blo] bli") << QStringLiteral("bla bla [blo] bli");
    // Test <https://www.kde.org|bla>
    QTest::newRow("[https://www.kde.org|bla]") << QStringLiteral(
        "bla [<a href=\"https://www.kde.org.|https://www.kde.org\">https://www.kde.org.|https://www.kde.org</a>]")
                                               << QStringLiteral("bla <a href=\"https://www.kde.org\">https://www.kde.org.</a>");
#if 0
    QTest::newRow("[https://www.kde.org|bla]") << QStringLiteral("[https://www.kde.org|https://www.kde.org/bla]")
                                               << QStringLiteral("<a href='https://www.kde.org/bla'>https://www.kde.org</a>");
    QTest::newRow("[https://www.kde.com/pages/viewpage.action?pageId=111111.|https://www.kde.com/pages/viewpage.action?pageId=111111]")
        << QStringLiteral("[https://www.kde.com//pages/viewpage.action?pageId=111111.|https://www.kde.com//pages/viewpage.action?pageId=111111]")
        << QStringLiteral("<a href='https://www.kde.com//pages/viewpage.action?pageId=111111'>https://www.kde.com//pages/viewpage.action?pageId=111111.</a>");
    QTest::newRow("blabla [https://www.kde.org|https://www.kde.org/bla]") << QStringLiteral("blabla [https://www.kde.org|https://www.kde.org/bla]")
                                                                          << QStringLiteral("blabla <a href='https://www.kde.org/bla'>https://www.kde.org</a>");

    QTest::newRow("blabla [https://www.kde.org|https://www.kde.org/bla] 2 ")
        << QStringLiteral(
               "ideas: [https://www.kde.com/pages/viewpage.action?pageId=11111.|https://www.kde.com/pages/viewpage.action?pageId=11111]\r\n [~vvvv] can")
        << QStringLiteral(
               "ideas: <a href='https://www.kde.com/pages/viewpage.action?pageId=11111'>https://www.kde.com/pages/viewpage.action?pageId=11111.</a>\r\n "
               "[~vvvv] can");
#endif
}

void UtilsTest::shouldConvertTextWithUrl()
{
    QFETCH(QString, text);
    QFETCH(QString, convertedText);
    QCOMPARE(Utils::convertTextWithUrl(text), convertedText);
}

void UtilsTest::shouldGenerateAvatarUrl_data()
{
    QTest::addColumn<QString>("serverUrl");
    QTest::addColumn<Utils::AvatarInfo>("avatarInfo");
    QTest::addColumn<QUrl>("result");
    {
        Utils::AvatarInfo avatarInfo;
        QTest::newRow("empty") << QString() << avatarInfo << QUrl();
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = QStringLiteral("user1");
        avatarInfo.avatarType = Utils::AvatarType::User;
        QTest::newRow("user1") << QStringLiteral("http://www.kde.org") << avatarInfo
                               << QUrl(QStringLiteral("http://www.kde.org/avatar/%1?format=png&size=22").arg(avatarInfo.identifier));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = QStringLiteral("user1");
        avatarInfo.avatarType = Utils::AvatarType::User;
        avatarInfo.etag = QStringLiteral("etag-user-identifier");
        QTest::newRow("user1-etag")
            << QStringLiteral("http://www.kde.org") << avatarInfo
            << QUrl(QStringLiteral("http://www.kde.org/avatar/%1?format=png&etag=%2&size=22").arg(avatarInfo.identifier, avatarInfo.etag));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = QStringLiteral("room1");
        avatarInfo.avatarType = Utils::AvatarType::Room;
        QTest::newRow("room1") << QStringLiteral("http://www.kde.org") << avatarInfo
                               << QUrl(QStringLiteral("http://www.kde.org/avatar/room/%1?format=png&size=22").arg(avatarInfo.identifier));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = QStringLiteral("room1");
        avatarInfo.avatarType = Utils::AvatarType::Room;
        avatarInfo.etag = QStringLiteral("etagIdentifier");
        QTest::newRow("room1-etag")
            << QStringLiteral("http://www.kde.org") << avatarInfo
            << QUrl(QStringLiteral("http://www.kde.org/avatar/room/%1?format=png&etag=%2&size=22").arg(avatarInfo.identifier, avatarInfo.etag));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = QStringLiteral("room1");
        avatarInfo.avatarType = Utils::AvatarType::Room;
        avatarInfo.etag = QStringLiteral("etagIdentifier");
        QTest::newRow("room1-etag-without-protocol")
            << QStringLiteral("www.kde.org") << avatarInfo
            << QUrl(QStringLiteral("https://www.kde.org/avatar/room/%1?format=png&etag=%2&size=22").arg(avatarInfo.identifier, avatarInfo.etag));
    }
}

void UtilsTest::shouldGenerateAvatarUrl()
{
    QFETCH(QString, serverUrl);
    QFETCH(Utils::AvatarInfo, avatarInfo);
    QFETCH(QUrl, result);
    QCOMPARE(Utils::avatarUrl(serverUrl, avatarInfo), result);
}

void UtilsTest::shouldTestAvatarInfoValues()
{
    Utils::AvatarInfo info;
    QVERIFY(!info.isValid());
    QVERIFY(info.etag.isEmpty());
    QVERIFY(info.identifier.isEmpty());
    QCOMPARE(info.avatarType, Utils::AvatarType::Unknown);

    info.identifier = QStringLiteral("bla");
    QCOMPARE(info.generateAvatarIdentifier(), QStringLiteral("bla"));
    info.etag = QStringLiteral("foo");
    QCOMPARE(info.generateAvatarIdentifier(), QStringLiteral("bla-foo"));
}

#include "moc_utilstest.cpp"

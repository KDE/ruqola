/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parserocketchaturlutilstest.h"
#include "parserocketchaturlutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(ParseRocketChatUrlUtilsTest)
ParseRocketChatUrlUtilsTest::ParseRocketChatUrlUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ParseRocketChatUrlUtilsTest::shouldHaveDefaultValues()
{
    ParseRocketChatUrlUtils w;
    QVERIFY(w.messageId().isEmpty());
    QVERIFY(w.roomId().isEmpty());
    QVERIFY(w.serverHost().isEmpty());
    QVERIFY(w.path().isEmpty());
}

void ParseRocketChatUrlUtilsTest::shouldParseUrl_data()
{
    QTest::addColumn<QString>("messageUrl");
    QTest::addColumn<ParseRocketChatUrlUtils::UrlType>("rocketChatUrlType");
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<QString>("roomId");
    QTest::addColumn<QString>("serverHost");
    QTest::addColumn<QString>("path");
    QTest::addColumn<ParseRocketChatUrlUtils::ChannelType>("channelType");
    QTest::addColumn<ParseRocketChatUrlUtils::RoomIdType>("roomIdType");

    QTest::addRow("empty") << QString() << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString() << QString()
                           << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde") << QStringLiteral("http://www.kde.org") << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString()
                         << QString() << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde-1") << QStringLiteral("https://www.kde.org") << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString()
                           << QString() << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde-2") << QStringLiteral("www.kde.org") << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString() << QString()
                           << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("gorocketchat") << QStringLiteral("https://go.rocket.chat/") << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString()
                                  << QString() << QString() << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("gorocketchat-1") << QStringLiteral(
        "https://go.rocket.chat/room?rid=NCrToCewka5MgMcDM&mid=Xope7b8WYWz82yHaq&host=www.kde.org&path=channel%2Ffoo%3Fmsg%3DXope7b8WYWz82yHaq")
                                    << ParseRocketChatUrlUtils::UrlType::Message << QStringLiteral("Xope7b8WYWz82yHaq") << QStringLiteral("NCrToCewka5MgMcDM")
                                    << QStringLiteral("www.kde.org") << QStringLiteral("channel/foo?msg=Xope7b8WYWz82yHaq")
                                    << ParseRocketChatUrlUtils::ChannelType::Channel << ParseRocketChatUrlUtils::RoomIdType::RoomId;

    QTest::addRow("gorocketchat-2") << QStringLiteral(
        "https://go.rocket.chat/"
        "room?rid=XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH&mid=Bo8pcAH86LxiYzu98&host=www.kde.org&path=direct%2FXQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH%3Fmsg%"
        "3DBo8pcAH86LxiYzu98") << ParseRocketChatUrlUtils::UrlType::Message
                                    << QStringLiteral("Bo8pcAH86LxiYzu98") << QStringLiteral("XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH")
                                    << QStringLiteral("www.kde.org") << QStringLiteral("direct/XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH?msg=Bo8pcAH86LxiYzu98")
                                    << ParseRocketChatUrlUtils::ChannelType::Direct << ParseRocketChatUrlUtils::RoomIdType::RoomId;

    QTest::addRow("rocketchatscheme-1") << QStringLiteral(
        "rocketchat://"
        "room?rid=XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH&mid=Bo8pcAH86LxiYzu98&host=www.kde.org&path=direct%2FXQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH%3Fmsg%"
        "3DBo8pcAH86LxiYzu98") << ParseRocketChatUrlUtils::UrlType::Message
                                        << QStringLiteral("Bo8pcAH86LxiYzu98") << QStringLiteral("XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH")
                                        << QStringLiteral("www.kde.org") << QStringLiteral("direct/XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH?msg=Bo8pcAH86LxiYzu98")
                                        << ParseRocketChatUrlUtils::ChannelType::Direct << ParseRocketChatUrlUtils::RoomIdType::RoomId;

    QTest::addRow("url-1") << QStringLiteral("https://www.kde.org/channel/python?msg=sn3gEQom7NcLxTg5h") << ParseRocketChatUrlUtils::UrlType::Message
                           << QStringLiteral("sn3gEQom7NcLxTg5h") << QStringLiteral("python") << QStringLiteral("www.kde.org")
                           << QStringLiteral("/channel/python") << ParseRocketChatUrlUtils::ChannelType::Channel
                           << ParseRocketChatUrlUtils::RoomIdType::RoomName;

    QTest::addRow("url-2") << QStringLiteral("https://www.kde.org/direct/python?msg=sn3gEQom7NcLxTg5h") << ParseRocketChatUrlUtils::UrlType::Message
                           << QStringLiteral("sn3gEQom7NcLxTg5h") << QStringLiteral("python") << QStringLiteral("www.kde.org")
                           << QStringLiteral("/direct/python") << ParseRocketChatUrlUtils::ChannelType::Direct << ParseRocketChatUrlUtils::RoomIdType::RoomName;
}

void ParseRocketChatUrlUtilsTest::shouldParseUrl()
{
    QFETCH(QString, messageUrl);
    QFETCH(ParseRocketChatUrlUtils::UrlType, rocketChatUrlType);
    QFETCH(QString, messageId);
    QFETCH(QString, roomId);
    QFETCH(QString, serverHost);
    QFETCH(QString, path);
    QFETCH(ParseRocketChatUrlUtils::ChannelType, channelType);
    QFETCH(ParseRocketChatUrlUtils::RoomIdType, roomIdType);

    ParseRocketChatUrlUtils w;
    QCOMPARE(w.parseUrl(messageUrl), rocketChatUrlType);
    QCOMPARE(w.messageId(), messageId);
    QCOMPARE(w.roomId(), roomId);
    QCOMPARE(w.serverHost(), serverHost);
    QCOMPARE(w.path(), path);
    QCOMPARE(w.channelType(), channelType);
    QCOMPARE(w.roomIdType(), roomIdType);
}

#include "moc_parserocketchaturlutilstest.cpp"

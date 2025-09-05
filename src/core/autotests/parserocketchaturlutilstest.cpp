/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parserocketchaturlutilstest.h"
using namespace Qt::Literals::StringLiterals;

#include "parserocketchaturlutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(ParseRocketChatUrlUtilsTest)
ParseRocketChatUrlUtilsTest::ParseRocketChatUrlUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ParseRocketChatUrlUtilsTest::shouldHaveDefaultValues()
{
    ParseRocketChatUrlUtils::ParsingInfo parseInfo;
    QVERIFY(parseInfo.messageId.isEmpty());
    QVERIFY(parseInfo.roomId.isEmpty());
    QVERIFY(parseInfo.serverHost.isEmpty());
    QVERIFY(parseInfo.path.isEmpty());
    QVERIFY(parseInfo.token.isEmpty());
    QVERIFY(parseInfo.userId.isEmpty());
    QCOMPARE(parseInfo.roomIdType, ParseRocketChatUrlUtils::RoomIdType::Unknown);
    QCOMPARE(parseInfo.channelType, ParseRocketChatUrlUtils::ChannelType::Unknown);
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
    QTest::addRow("kde") << u"http://www.kde.org"_s << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString() << QString()
                         << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde-1") << u"https://www.kde.org"_s << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString() << QString()
                           << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde-2") << u"www.kde.org"_s << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString() << QString()
                           << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("gorocketchat") << u"https://go.rocket.chat/"_s << ParseRocketChatUrlUtils::UrlType::Unknown << QString() << QString() << QString()
                                  << QString() << ParseRocketChatUrlUtils::ChannelType::Unknown << ParseRocketChatUrlUtils::RoomIdType::Unknown;
    QTest::addRow("gorocketchat-1") << QStringLiteral(
        "https://go.rocket.chat/room?rid=NCrToCewka5MgMcDM&mid=Xope7b8WYWz82yHaq&host=www.kde.org&path=channel%2Ffoo%3Fmsg%3DXope7b8WYWz82yHaq")
                                    << ParseRocketChatUrlUtils::UrlType::Message << u"Xope7b8WYWz82yHaq"_s << u"NCrToCewka5MgMcDM"_s << u"www.kde.org"_s
                                    << u"channel/foo?msg=Xope7b8WYWz82yHaq"_s << ParseRocketChatUrlUtils::ChannelType::Channel
                                    << ParseRocketChatUrlUtils::RoomIdType::RoomId;

    QTest::addRow("gorocketchat-2") << QStringLiteral(
        "https://go.rocket.chat/"
        "room?rid=XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH&mid=Bo8pcAH86LxiYzu98&host=www.kde.org&path=direct%2FXQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH%3Fmsg%"
        "3DBo8pcAH86LxiYzu98") << ParseRocketChatUrlUtils::UrlType::Message
                                    << u"Bo8pcAH86LxiYzu98"_s << u"XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH"_s << u"www.kde.org"_s
                                    << u"direct/XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH?msg=Bo8pcAH86LxiYzu98"_s << ParseRocketChatUrlUtils::ChannelType::Direct
                                    << ParseRocketChatUrlUtils::RoomIdType::RoomId;

    QTest::addRow("rocketchatscheme-1") << QStringLiteral(
        "rocketchat://"
        "room?rid=XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH&mid=Bo8pcAH86LxiYzu98&host=www.kde.org&path=direct%2FXQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH%3Fmsg%"
        "3DBo8pcAH86LxiYzu98") << ParseRocketChatUrlUtils::UrlType::Message
                                        << u"Bo8pcAH86LxiYzu98"_s << u"XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH"_s << u"www.kde.org"_s
                                        << u"direct/XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH?msg=Bo8pcAH86LxiYzu98"_s << ParseRocketChatUrlUtils::ChannelType::Direct
                                        << ParseRocketChatUrlUtils::RoomIdType::RoomId;

    QTest::addRow("url-1") << u"https://www.kde.org/channel/python?msg=sn3gEQom7NcLxTg5h"_s << ParseRocketChatUrlUtils::UrlType::Message
                           << u"sn3gEQom7NcLxTg5h"_s << u"python"_s << u"www.kde.org"_s << u"/channel/python"_s << ParseRocketChatUrlUtils::ChannelType::Channel
                           << ParseRocketChatUrlUtils::RoomIdType::RoomName;

    QTest::addRow("url-2") << u"https://www.kde.org/direct/python?msg=sn3gEQom7NcLxTg5h"_s << ParseRocketChatUrlUtils::UrlType::Message
                           << u"sn3gEQom7NcLxTg5h"_s << u"python"_s << u"www.kde.org"_s << u"/direct/python"_s << ParseRocketChatUrlUtils::ChannelType::Direct
                           << ParseRocketChatUrlUtils::RoomIdType::RoomName;

    QTest::addRow("invite1") << u"https://go.rocket.chat/invite?host=foo.chat.kde.com&path=invite%2FQ67r9r"_s << ParseRocketChatUrlUtils::UrlType::Invite
                             << QString() << QString() << u"foo.chat.kde.com"_s << u"invite/Q67r9r"_s << ParseRocketChatUrlUtils::ChannelType::Unknown
                             << ParseRocketChatUrlUtils::RoomIdType::Unknown;
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
    QCOMPARE(w.parsingInfo().messageId, messageId);
    QCOMPARE(w.parsingInfo().roomId, roomId);
    QCOMPARE(w.parsingInfo().serverHost, serverHost);
    QCOMPARE(w.parsingInfo().path, path);
    QCOMPARE(w.parsingInfo().channelType, channelType);
    QCOMPARE(w.parsingInfo().roomIdType, roomIdType);
}

#include "moc_parserocketchaturlutilstest.cpp"

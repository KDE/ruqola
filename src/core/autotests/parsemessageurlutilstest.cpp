/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsemessageurlutilstest.h"
#include "parsemessageurlutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(ParseMessageUrlUtilsTest)
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
    QTest::addColumn<ParseMessageUrlUtils::ChannelType>("channelType");
    QTest::addColumn<ParseMessageUrlUtils::RoomIdType>("roomIdType");

    QTest::addRow("empty") << QString() << false << QString() << QString() << QString() << QString() << ParseMessageUrlUtils::ChannelType::Unknown
                           << ParseMessageUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde") << QStringLiteral("http://www.kde.org") << false << QString() << QString() << QString() << QString()
                         << ParseMessageUrlUtils::ChannelType::Unknown << ParseMessageUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde-1") << QStringLiteral("https://www.kde.org") << false << QString() << QString() << QString() << QString()
                           << ParseMessageUrlUtils::ChannelType::Unknown << ParseMessageUrlUtils::RoomIdType::Unknown;
    QTest::addRow("kde-2") << QStringLiteral("www.kde.org") << false << QString() << QString() << QString() << QString()
                           << ParseMessageUrlUtils::ChannelType::Unknown << ParseMessageUrlUtils::RoomIdType::Unknown;
    QTest::addRow("gorocketchat") << QStringLiteral("https://go.rocket.chat/") << false << QString() << QString() << QString() << QString()
                                  << ParseMessageUrlUtils::ChannelType::Unknown << ParseMessageUrlUtils::RoomIdType::Unknown;
    QTest::addRow("gorocketchat-1") << QStringLiteral(
        "https://go.rocket.chat/room?rid=NCrToCewka5MgMcDM&mid=Xope7b8WYWz82yHaq&host=www.kde.org&path=channel%2Ffoo%3Fmsg%3DXope7b8WYWz82yHaq")
                                    << true << QStringLiteral("Xope7b8WYWz82yHaq") << QStringLiteral("NCrToCewka5MgMcDM") << QStringLiteral("www.kde.org")
                                    << QStringLiteral("channel/foo?msg=Xope7b8WYWz82yHaq") << ParseMessageUrlUtils::ChannelType::Channel
                                    << ParseMessageUrlUtils::RoomIdType::RoomId;

    QTest::addRow("gorocketchat-2") << QStringLiteral(
        "https://go.rocket.chat/"
        "room?rid=XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH&mid=Bo8pcAH86LxiYzu98&host=www.kde.org&path=direct%2FXQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH%3Fmsg%"
        "3DBo8pcAH86LxiYzu98") << true
                                    << QStringLiteral("Bo8pcAH86LxiYzu98") << QStringLiteral("XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH")
                                    << QStringLiteral("www.kde.org") << QStringLiteral("direct/XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH?msg=Bo8pcAH86LxiYzu98")
                                    << ParseMessageUrlUtils::ChannelType::Direct << ParseMessageUrlUtils::RoomIdType::RoomId;

    QTest::addRow("url-1") << QStringLiteral("https://www.kde.org/channel/python?msg=sn3gEQom7NcLxTg5h") << true << QStringLiteral("sn3gEQom7NcLxTg5h")
                           << QStringLiteral("python") << QStringLiteral("www.kde.org") << QStringLiteral("/channel/python")
                           << ParseMessageUrlUtils::ChannelType::Channel << ParseMessageUrlUtils::RoomIdType::RoomName;

    QTest::addRow("url-2") << QStringLiteral("https://www.kde.org/direct/python?msg=sn3gEQom7NcLxTg5h") << true << QStringLiteral("sn3gEQom7NcLxTg5h")
                           << QStringLiteral("python") << QStringLiteral("www.kde.org") << QStringLiteral("/direct/python")
                           << ParseMessageUrlUtils::ChannelType::Direct << ParseMessageUrlUtils::RoomIdType::RoomName;
}

void ParseMessageUrlUtilsTest::shouldParseUrl()
{
    QFETCH(QString, messageUrl);
    QFETCH(bool, parsingValid);
    QFETCH(QString, messageId);
    QFETCH(QString, roomId);
    QFETCH(QString, serverHost);
    QFETCH(QString, path);
    QFETCH(ParseMessageUrlUtils::ChannelType, channelType);
    QFETCH(ParseMessageUrlUtils::RoomIdType, roomIdType);

    ParseMessageUrlUtils w;
    QCOMPARE(w.parseUrl(messageUrl), parsingValid);
    QCOMPARE(w.messageId(), messageId);
    QCOMPARE(w.roomId(), roomId);
    QCOMPARE(w.serverHost(), serverHost);
    QCOMPARE(w.path(), path);
    QCOMPARE(w.channelType(), channelType);
    QCOMPARE(w.roomIdType(), roomIdType);
}

#include "moc_parsemessageurlutilstest.cpp"

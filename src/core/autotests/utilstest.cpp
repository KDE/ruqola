/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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
#include <KColorScheme>
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
    QTest::newRow("notification1") << QStringLiteral("notification") << QStringLiteral("title") << QStringLiteral("pong") << QStringLiteral("tgrk5CZKgYGiSSqXp")
                                   << QString() << QStringLiteral("d") << QStringLiteral("Laurent") << QStringLiteral("laurent");
    // TODO
    QTest::newRow("notificationencrypted") << QStringLiteral("notificationencrypted") << QStringLiteral("title") << QStringLiteral("pong")
                                           << QStringLiteral("tgrk5CZKgYGiSSqXp") << QString() << QStringLiteral("d") << QStringLiteral("foo")
                                           << QStringLiteral("bla");

    QTest::newRow("notification2") << QStringLiteral("notification1") << QStringLiteral("my title") << QStringLiteral("@here")
                                   << QStringLiteral("Gsvg6BGoBfmPLoFie") << QStringLiteral("roomname example") << QStringLiteral("c") << QStringLiteral("foo")
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
    QTest::newRow("test4") << QStringLiteral("bla bla [blo](http://www.kde.org) bli [blu](http://www.kdi.org)")
                           << QStringLiteral("bla bla <a href='http://www.kde.org'>blo</a> bli <a href='http://www.kdi.org'>blu</a>");
    QTest::newRow("test5") << QStringLiteral("bla bla [blo]") << QStringLiteral("bla bla [blo]");
    QTest::newRow("test6") << QStringLiteral("bla bla [blo] bli") << QStringLiteral("bla bla [blo] bli");

    // Test <https://www.kde.org|bla>
    // QTest::newRow("https://www.kde.org|bla") << QStringLiteral("<https://www.kde.org|bla>") << QStringLiteral("<a href='https://www.kde.org'>bla</a>");
    // QTest::newRow("https://www.kde.org|bla 2 ") << QStringLiteral("bli <https://www.kde.org|bla> blu") << QStringLiteral("bli <a
    // href='https://www.kde.org'>bla</a> blu");
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

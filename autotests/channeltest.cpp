/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "channeltest.h"
#include "channel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

QTEST_GUILESS_MAIN(ChannelTest)

ChannelTest::ChannelTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelTest::shouldHaveDefaultValue()
{
    Channel t;
    QCOMPARE(t.type(), Channel::ChannelType::Unknown);
    QVERIFY(t.roomName().isEmpty());
    QVERIFY(t.roomId().isEmpty());
    QVERIFY(t.roomType().isEmpty());

    QVERIFY(t.userId().isEmpty());
    QVERIFY(t.name().isEmpty());
    QVERIFY(t.status().isEmpty());
    QVERIFY(t.userName().isEmpty());
    QVERIFY(!t.iconFromStatus().isEmpty());
}

void ChannelTest::shouldAssignValue()
{
}

void ChannelTest::shouldCopyValue()
{
}

void ChannelTest::shouldParseChannel_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Channel::ChannelType>("channelType");
    QTest::addColumn<Channel>("expectedChannel");

    Channel chanRoom;
    chanRoom.setRoomId(QStringLiteral("aH6LfuQz4f7x9rMb"));
    chanRoom.setRoomName(QStringLiteral("qt"));
    chanRoom.setRoomType(QStringLiteral("c"));
    chanRoom.setType(Channel::ChannelType::Room);

    QTest::newRow("room") << QStringLiteral("channelroom") << Channel::ChannelType::Room << chanRoom;

    Channel chanUser;
    chanUser.setName(QStringLiteral("foo"));
    chanUser.setUserName(QStringLiteral("bla"));
    chanUser.setUserId(QStringLiteral("o7kiLAYPCiDidqJe"));
    chanUser.setStatus(QStringLiteral("online"));
    chanUser.setType(Channel::ChannelType::PrivateChannel);

    QTest::newRow("user") << QStringLiteral("channeluser") << Channel::ChannelType::PrivateChannel << chanUser;
}

void ChannelTest::shouldParseChannel()
{
    QFETCH(QString, name);
    QFETCH(Channel::ChannelType, channelType);
    QFETCH(Channel, expectedChannel);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    Channel originalChannel;
    originalChannel.parseChannel(obj, channelType);
    const bool channelIsEqual = (originalChannel == expectedChannel);
    if (!channelIsEqual) {
        qDebug() << "originalChannel " << originalChannel;
        qDebug() << "ExpectedChannel " << expectedChannel;
    }
    QVERIFY(channelIsEqual);
}

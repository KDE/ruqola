/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeltest.h"
#include "channel.h"
#include "ruqola_autotest_helper.h"
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
    chanUser.setType(Channel::ChannelType::DirectChannel);

    QTest::newRow("user") << QStringLiteral("channeluser") << Channel::ChannelType::DirectChannel << chanUser;
}

void ChannelTest::shouldParseChannel()
{
    QFETCH(QString, name);
    QFETCH(Channel::ChannelType, channelType);
    QFETCH(Channel, expectedChannel);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    Channel originalChannel;
    originalChannel.parseChannel(obj, channelType);
    const bool channelIsEqual = (originalChannel == expectedChannel);
    if (!channelIsEqual) {
        qDebug() << "originalChannel " << originalChannel;
        qDebug() << "ExpectedChannel " << expectedChannel;
    }
    QVERIFY(channelIsEqual);
}

#include "moc_channeltest.cpp"

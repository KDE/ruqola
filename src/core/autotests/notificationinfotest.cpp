/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationinfotest.h"
#include "notificationinfo.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(NotificationInfoTest)

NotificationInfoTest::NotificationInfoTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationInfoTest::shouldHaveDefaultValues()
{
    NotificationInfo t;
    QVERIFY(t.accountName().isEmpty());
    QVERIFY(t.message().isEmpty());
    QVERIFY(t.title().isEmpty());
    QVERIFY(t.senderId().isEmpty());
    QVERIFY(t.senderName().isEmpty());
    QVERIFY(t.senderUserName().isEmpty());
    QVERIFY(t.roomName().isEmpty());
    QVERIFY(t.roomId().isEmpty());
    QVERIFY(t.channelType().isEmpty());
    QVERIFY(t.tmId().isEmpty());
    QVERIFY(t.pixmap().isNull());
    QVERIFY(t.messageId().isEmpty());
    QCOMPARE(t.notificationType(), NotificationInfo::NotificationType::StandardMessage);
}

void NotificationInfoTest::shouldParseNotification_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("message");
    QTest::addColumn<QByteArray>("sender");
    QTest::addColumn<QString>("roomName");
    QTest::addColumn<QString>("channelType");
    QTest::addColumn<QString>("senderName");
    QTest::addColumn<QString>("senderUserName");
    QTest::addColumn<QByteArray>("roomId");
    QTest::addColumn<QByteArray>("tmId");
    QTest::addColumn<QByteArray>("messageId");
    QTest::newRow("notification1") << QStringLiteral("notification") << QStringLiteral("title") << QStringLiteral("pong")
                                   << QByteArrayLiteral("tgrk5CZKgYGiSSqXp") << QString() << QStringLiteral("d") << QStringLiteral("Laurent")
                                   << QStringLiteral("laurent") << QByteArrayLiteral("tgrk5CZKgYGiSSqXpuKK39zoewTkdacidH") << QByteArray()
                                   << QByteArrayLiteral("yfitiEkpGyDAAJ6Eo");
    // TODO
    QTest::newRow("notificationencrypted") << QStringLiteral("notificationencrypted") << QStringLiteral("title") << QStringLiteral("pong")
                                           << QByteArrayLiteral("tgrk5CZKgYGiSSqXp") << QString() << QStringLiteral("d") << QStringLiteral("foo")
                                           << QStringLiteral("bla") << QByteArrayLiteral("XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH") << QByteArray()
                                           << QByteArrayLiteral("PuKLfJfd5r5DrZBdN");

    QTest::newRow("notification2") << QStringLiteral("notification1") << QStringLiteral("my title") << QStringLiteral("@here")
                                   << QByteArrayLiteral("Gsvg6BGoBfmPLoFie") << QStringLiteral("roomname example") << QStringLiteral("c")
                                   << QStringLiteral("foo") << QStringLiteral("foo.bla") << QByteArrayLiteral("Fc4EWcQXCkJkJ7QEQ") << QByteArray()
                                   << QByteArrayLiteral("dNYhFAYBhqPFnBwRy");

    QTest::newRow("notification-tmid1") << QStringLiteral("notification-tmid1") << QStringLiteral("my title") << QStringLiteral("@here")
                                        << QByteArrayLiteral("Gsvg6BGoBfmPLoFie") << QStringLiteral("roomname example") << QStringLiteral("c")
                                        << QStringLiteral("foo") << QStringLiteral("foo.bla") << QByteArrayLiteral("Fc4EWcQXCkJkJ7QEQ")
                                        << QByteArrayLiteral("QLNcu8TnRi8Ld57Pk") << QByteArrayLiteral("dNYhFAYBhqPFnBwRy");
}

void NotificationInfoTest::shouldParseNotification()
{
    QFETCH(QString, fileName);
    QFETCH(QString, title);
    QFETCH(QString, message);
    QFETCH(QByteArray, sender);
    QFETCH(QString, roomName);
    QFETCH(QString, channelType);
    QFETCH(QString, senderName);
    QFETCH(QString, senderUserName);
    QFETCH(QByteArray, roomId);
    QFETCH(QByteArray, tmId);
    QFETCH(QByteArray, messageId);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + QLatin1StringView("/json/") + fileName + QLatin1StringView(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object().value(QLatin1StringView("fields")).toObject();
    const QJsonArray contents = fields.value(QLatin1StringView("args")).toArray();

    NotificationInfo info;
    info.parseNotification(contents);
    QEXPECT_FAIL("notificationencrypted", "Encrypted message not supported yet", Continue);
    QCOMPARE(info.message(), message);
    QCOMPARE(info.title(), title);
    QEXPECT_FAIL("notificationencrypted", "Encrypted message not supported yet", Continue);
    QCOMPARE(info.senderId(), sender);
    QCOMPARE(info.roomName(), roomName);
    QCOMPARE(info.roomId(), roomId);
    QCOMPARE(info.channelType(), channelType);
    QCOMPARE(info.senderUserName(), senderUserName);
    QCOMPARE(info.senderName(), senderName);
    QCOMPARE(info.tmId(), tmId);
    QCOMPARE(info.messageId(), messageId);

    QVERIFY(info.isValid());
}

#include "moc_notificationinfotest.cpp"

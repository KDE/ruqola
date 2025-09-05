/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationinfotest.h"
#include "notificationinfo.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    QTest::newRow("notification1") << u"notification"_s << u"title"_s << u"pong"_s << QByteArrayLiteral("tgrk5CZKgYGiSSqXp") << QString() << u"d"_s
                                   << u"Laurent"_s << u"laurent"_s << QByteArrayLiteral("tgrk5CZKgYGiSSqXpuKK39zoewTkdacidH") << QByteArray()
                                   << QByteArrayLiteral("yfitiEkpGyDAAJ6Eo");
    // TODO
    QTest::newRow("notificationencrypted") << u"notificationencrypted"_s << u"title"_s << u"pong"_s << QByteArrayLiteral("tgrk5CZKgYGiSSqXp") << QString()
                                           << u"d"_s << u"foo"_s << u"bla"_s << QByteArrayLiteral("XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH") << QByteArray()
                                           << QByteArrayLiteral("PuKLfJfd5r5DrZBdN");

    QTest::newRow("notification2") << u"notification1"_s << u"my title"_s << u"@here"_s << QByteArrayLiteral("Gsvg6BGoBfmPLoFie") << u"roomname example"_s
                                   << u"c"_s << u"foo"_s << u"foo.bla"_s << QByteArrayLiteral("Fc4EWcQXCkJkJ7QEQ") << QByteArray()
                                   << QByteArrayLiteral("dNYhFAYBhqPFnBwRy");

    QTest::newRow("notification-tmid1") << u"notification-tmid1"_s << u"my title"_s << u"@here"_s << QByteArrayLiteral("Gsvg6BGoBfmPLoFie")
                                        << u"roomname example"_s << u"c"_s << u"foo"_s << u"foo.bla"_s << QByteArrayLiteral("Fc4EWcQXCkJkJ7QEQ")
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
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + fileName + ".json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object().value("fields"_L1).toObject();
    const QJsonArray contents = fields.value("args"_L1).toArray();

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

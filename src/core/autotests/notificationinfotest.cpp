/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "notificationinfotest.h"
#include "notificationinfo.h"
#include <QJsonArray>
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
    const NotificationInfo t;
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
    QVERIFY(!t.forceShowAccountName());
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
    QTest::newRow("notification1") << u"notification"_s << u"title"_s << u"pong"_s << "tgrk5CZKgYGiSSqXp"_ba << QString() << u"d"_s << u"Laurent"_s
                                   << u"laurent"_s << "tgrk5CZKgYGiSSqXpuKK39zoewTkdacidH"_ba << QByteArray() << "yfitiEkpGyDAAJ6Eo"_ba;
    // TODO
    QTest::newRow("notificationencrypted") << u"notificationencrypted"_s << u"title"_s << u"pong"_s << "tgrk5CZKgYGiSSqXp"_ba << QString() << u"d"_s << u"foo"_s
                                           << u"bla"_s << "XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH"_ba << QByteArray() << "PuKLfJfd5r5DrZBdN"_ba;

    QTest::newRow("notification2") << u"notification1"_s << u"my title"_s << u"@here"_s << "Gsvg6BGoBfmPLoFie"_ba << u"roomname example"_s << u"c"_s << u"foo"_s
                                   << u"foo.bla"_s << "Fc4EWcQXCkJkJ7QEQ"_ba << QByteArray() << "dNYhFAYBhqPFnBwRy"_ba;

    QTest::newRow("notification-tmid1") << u"notification-tmid1"_s << u"my title"_s << u"@here"_s << "Gsvg6BGoBfmPLoFie"_ba << u"roomname example"_s << u"c"_s
                                        << u"foo"_s << u"foo.bla"_s << "Fc4EWcQXCkJkJ7QEQ"_ba
                                        << "QLNcu8TnRi8Ld57Pk"_ba << "dNYhFAYBhqPFnBwRy"_ba;
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

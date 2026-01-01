/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationoptionstest.h"
#include "notifications/notificationoptions.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(NotificationOptionsTest)

NotificationOptionsTest::NotificationOptionsTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationOptionsTest::shouldHaveDefaultValue()
{
    NotificationOptions w;
    QVERIFY(w.desktopNotifications().isEmpty());
    QVERIFY(w.mobilePushNotification().isEmpty());
    QVERIFY(w.emailNotifications().isEmpty());
    QVERIFY(w.unreadTrayIconAlert().isEmpty());
    QVERIFY(!w.disableNotifications());
    QVERIFY(!w.hideUnreadStatus());
    QVERIFY(!w.muteGroupMentions());
    QVERIFY(w.audioNotificationValue().isEmpty());
    // 26/03/2024: size: 200
    QCOMPARE(sizeof(NotificationOptions), 200);
}

void NotificationOptionsTest::shouldAssignValue()
{
    NotificationOptions w;
    NotificationOptions::NotificationValue desktopNotifications = NotificationOptions::NotificationValue{"bla"_ba, QByteArray()};
    w.setDesktopNotifications(desktopNotifications);

    NotificationOptions::NotificationValue mobilePushNotification = NotificationOptions::NotificationValue{"bli"_ba, QByteArray()};
    w.setMobilePushNotification(mobilePushNotification);

    NotificationOptions::NotificationValue emailNotifications = NotificationOptions::NotificationValue{"blu"_ba, QByteArray()};
    w.setEmailNotifications(emailNotifications);

    QString unreadTrayIconAlert = u"Ablu"_s;
    w.setUnreadTrayIconAlert(unreadTrayIconAlert);

    QByteArray audioNotificationValue = "ZZZZ"_ba;
    w.setAudioNotificationValue(audioNotificationValue);

    bool disableNotifications = true;
    w.setDisableNotifications(disableNotifications);

    bool hideUnreadStatus = true;
    w.setHideUnreadStatus(hideUnreadStatus);

    bool muteGroupMentions = true;
    w.setMuteGroupMentions(muteGroupMentions);

    QCOMPARE(w.desktopNotifications(), desktopNotifications);
    QCOMPARE(w.mobilePushNotification(), mobilePushNotification);
    QCOMPARE(w.emailNotifications(), emailNotifications);
    QCOMPARE(w.unreadTrayIconAlert(), unreadTrayIconAlert);
    QCOMPARE(w.audioNotificationValue(), audioNotificationValue);
    QCOMPARE(w.disableNotifications(), disableNotifications);
    QCOMPARE(w.hideUnreadStatus(), hideUnreadStatus);
    QCOMPARE(w.muteGroupMentions(), muteGroupMentions);

    NotificationOptions t;
    t = w;
    QCOMPARE(t, w);
}

void NotificationOptionsTest::shouldParseNotification_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QByteArray>("desktopNotifications");
    QTest::addColumn<QByteArray>("mobilePushNotification");
    QTest::addColumn<QByteArray>("emailNotifications");
    NotificationOptions notif;
    QTest::addRow("notification1") << u"notification1"_s << QByteArrayLiteral("default") << QByteArrayLiteral("all") << "all"_ba;
}

void NotificationOptionsTest::shouldParseNotification()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QByteArray, desktopNotifications);
    QFETCH(QByteArray, mobilePushNotification);
    QFETCH(QByteArray, emailNotifications);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/notificationoption/"_L1 + fileNameinit + ".json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject notification = doc.object();

    NotificationOptions w;
    w.parseNotificationOptions(notification);
    QCOMPARE(w.desktopNotifications().currentValue(), desktopNotifications);
    QCOMPARE(w.mobilePushNotification().currentValue(), mobilePushNotification);
    QCOMPARE(w.emailNotifications().currentValue(), emailNotifications);
}

#include "moc_notificationoptionstest.cpp"

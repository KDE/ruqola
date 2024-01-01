/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationoptionstest.h"
#include "notifications/notificationoptions.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
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
}

void NotificationOptionsTest::shouldAssignValue()
{
    NotificationOptions w;
    NotificationOptions::NotificationValue desktopNotifications = NotificationOptions::NotificationValue{QStringLiteral("bla"), QString()};
    w.setDesktopNotifications(desktopNotifications);

    NotificationOptions::NotificationValue mobilePushNotification = NotificationOptions::NotificationValue{QStringLiteral("bli"), QString()};
    w.setMobilePushNotification(mobilePushNotification);

    NotificationOptions::NotificationValue emailNotifications = NotificationOptions::NotificationValue{QStringLiteral("blu"), QString()};
    w.setEmailNotifications(emailNotifications);

    QString unreadTrayIconAlert = QStringLiteral("Ablu");
    w.setUnreadTrayIconAlert(unreadTrayIconAlert);

    QString audioNotificationValue = QStringLiteral("ZZZZ");
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
    QTest::addColumn<QString>("desktopNotifications");
    QTest::addColumn<QString>("mobilePushNotification");
    QTest::addColumn<QString>("emailNotifications");
    NotificationOptions notif;
    QTest::addRow("notification1") << QStringLiteral("notification1") << QStringLiteral("default") << QStringLiteral("all") << QStringLiteral("all");
}

void NotificationOptionsTest::shouldParseNotification()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QString, desktopNotifications);
    QFETCH(QString, mobilePushNotification);
    QFETCH(QString, emailNotifications);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/notificationoption/") + fileNameinit + QLatin1String(".json");
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

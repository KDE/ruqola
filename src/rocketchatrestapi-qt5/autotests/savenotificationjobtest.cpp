/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "savenotificationjobtest.h"
#include "rooms/savenotificationjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(SaveNotificationJobTest)
using namespace RocketChatRestApi;
SaveNotificationJobTest::SaveNotificationJobTest(QObject *parent)
    : QObject(parent)
{
}

void SaveNotificationJobTest::shouldHaveDefaultValue()
{
    SaveNotificationJob job;
    verifyDefaultValue(&job);

    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.disableNotifications());
    QVERIFY(!job.muteGroupMentions());
    QVERIFY(!job.hideUnreadStatus());
    QVERIFY(!job.hideMentionStatus());
    QVERIFY(job.emailNotifications().isEmpty());
    QVERIFY(job.desktopNotifications().isEmpty());
    QVERIFY(job.mobilePushNotifications().isEmpty());
    QVERIFY(job.audioNotificationValue().isEmpty());
    QCOMPARE(job.desktopNotificationDuration(), 0);
    QVERIFY(job.unreadAlert().isEmpty());

    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SaveNotificationJobTest::shouldGenerateRequest()
{
    SaveNotificationJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.saveNotification")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SaveNotificationJobTest::shouldGenerateJson()
{
    SaveNotificationJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"notifications":{},"roomId":"%1"})").arg(roomId).toLatin1());

    // Add settings
    const bool hideUnread = true;
    job.setHideUnreadStatus(hideUnread);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"notifications":{"hideUnreadStatus":"1"},"roomId":"%1"})").arg(roomId).toLatin1());
    const QString mobilePushNotifications = QStringLiteral("all");
    job.setMobilePushNotifications(mobilePushNotifications);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{"
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\"},\"roomId\":\"%1\"}")
                 .arg(roomId, mobilePushNotifications)
                 .toLatin1());

    const bool disableNotifications = true;
    job.setDisableNotifications(disableNotifications);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{"
                            "\"disableNotifications\":\"1\","
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\"},\"roomId\":\"%1\"}")
                 .arg(roomId, mobilePushNotifications)
                 .toLatin1());

    const bool muteMentionGroups = true;
    job.setMuteGroupMentions(muteMentionGroups);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{"
                            "\"disableNotifications\":\"1\","
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\","
                            "\"muteGroupMentions\":\"1\"},\"roomId\":\"%1\"}")
                 .arg(roomId, mobilePushNotifications)
                 .toLatin1());

    // TODO add more settings
}

void SaveNotificationJobTest::shouldNotStarting()
{
    SaveNotificationJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());

    // We need to change a settings
    job.setAudioNotificationValue(QStringLiteral("foo"));
    QVERIFY(job.canStart());
}

#include "moc_savenotificationjobtest.cpp"

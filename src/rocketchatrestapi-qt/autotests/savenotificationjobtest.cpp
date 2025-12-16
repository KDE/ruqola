/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "savenotificationjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rooms/savenotificationjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);

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
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.saveNotification"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SaveNotificationJobTest::shouldGenerateJson()
{
    SaveNotificationJob job;
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"notifications":{},"roomId":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());

    // Add settings
    const bool hideUnread = true;
    job.setHideUnreadStatus(hideUnread);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"notifications":{"hideUnreadStatus":"1"},"roomId":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());
    const QString mobilePushNotifications = u"all"_s;
    job.setMobilePushNotifications(mobilePushNotifications);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{"
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\"},\"roomId\":\"%1\"}")
                 .arg(QLatin1StringView(roomId), mobilePushNotifications)
                 .toLatin1());

    const bool disableNotifications = true;
    job.setDisableNotifications(disableNotifications);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{"
                            "\"disableNotifications\":\"1\","
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\"},\"roomId\":\"%1\"}")
                 .arg(QLatin1StringView(roomId), mobilePushNotifications)
                 .toLatin1());

    const bool muteMentionGroups = true;
    job.setMuteGroupMentions(muteMentionGroups);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{"
                            "\"disableNotifications\":\"1\","
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\","
                            "\"muteGroupMentions\":\"1\"},\"roomId\":\"%1\"}")
                 .arg(QLatin1StringView(roomId), mobilePushNotifications)
                 .toLatin1());

    // TODO add more settings
}

void SaveNotificationJobTest::shouldNotStarting()
{
    SaveNotificationJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());

    // We need to change a settings
    job.setAudioNotificationValue(u"foo"_s);
    QVERIFY(job.canStart());
}

#include "moc_savenotificationjobtest.cpp"

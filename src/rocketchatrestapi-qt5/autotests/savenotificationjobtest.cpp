/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "savenotificationjobtest.h"
#include "rooms/savenotificationjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
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
    QVERIFY(job.emailNotifications().isEmpty());
    QVERIFY(job.audioNotifications().isEmpty());
    QVERIFY(job.mobilePushNotifications().isEmpty());
    QVERIFY(job.audioNotificationValue().isEmpty());
    QCOMPARE(job.desktopNotificationDuration(), 0);
    QVERIFY(job.unreadAlert().isEmpty());

    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"notifications\":{},\"roomId\":\"%1\"}").arg(roomId).toLatin1());

    //Add settings
    const QString audioNotification = QStringLiteral("all");
    job.setAudioNotifications(audioNotification);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{\"audioNotifications\":\"%2\"},\"roomId\":\"%1\"}")
             .arg(roomId, audioNotification).toLatin1());

    const bool hideUnread = true;
    job.setHideUnreadStatus(hideUnread);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{\"audioNotifications\":\"%2\",\"hideUnreadStatus\":\"1\"},\"roomId\":\"%1\"}")
             .arg(roomId, audioNotification).toLatin1());
    const QString mobilePushNotifications = QStringLiteral("all");
    job.setMobilePushNotifications(mobilePushNotifications);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{\"audioNotifications\":\"%2\","
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\"},\"roomId\":\"%1\"}")
             .arg(roomId, audioNotification, mobilePushNotifications).toLatin1());

    const bool disableNotifications = true;
    job.setDisableNotifications(disableNotifications);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{\"audioNotifications\":\"%2\","
                            "\"disableNotifications\":\"1\","
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\"},\"roomId\":\"%1\"}")
             .arg(roomId, audioNotification, mobilePushNotifications).toLatin1());

    const bool muteMentionGroups = true;
    job.setMuteGroupMentions(muteMentionGroups);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"notifications\":{\"audioNotifications\":\"%2\","
                            "\"disableNotifications\":\"1\","
                            "\"hideUnreadStatus\":\"1\","
                            "\"mobilePushNotifications\":\"%3\","
                            "\"muteGroupMentions\":\"1\"},\"roomId\":\"%1\"}")
             .arg(roomId, audioNotification, mobilePushNotifications).toLatin1());

    //TODO add more settings
}

void SaveNotificationJobTest::shouldNotStarting()
{
    SaveNotificationJob job;

    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);

    QNetworkAccessManager *mNetworkAccessManager = new QNetworkAccessManager;
    job.setNetworkAccessManager(mNetworkAccessManager);
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

    //We need to change a settings
    job.setAudioNotificationValue(QStringLiteral("foo"));
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}

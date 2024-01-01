/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelannouncementjobtest.h"
#include "channels/changechannelannouncementjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeChannelAnnouncementJobTest)
using namespace RocketChatRestApi;
ChangeChannelAnnouncementJobTest::ChangeChannelAnnouncementJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelAnnouncementJobTest::shouldNotStarting()
{
    ChangeChannelAnnouncementJob job;

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
    QVERIFY(job.canStart());
}

void ChangeChannelAnnouncementJobTest::shouldHaveDefaultValue()
{
    ChangeChannelAnnouncementJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.announcement().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelAnnouncementJobTest::shouldGenerateRequest()
{
    ChangeChannelAnnouncementJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setAnnouncement")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeChannelAnnouncementJobTest::shouldGenerateJson()
{
    ChangeChannelAnnouncementJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString announcement = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setAnnouncement(announcement);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"announcement":"%1","roomId":"%2"})").arg(announcement, roomId).toLatin1());
}

#include "moc_changechannelannouncementjobtest.cpp"

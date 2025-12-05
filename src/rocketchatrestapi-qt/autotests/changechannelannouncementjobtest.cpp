/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelannouncementjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

void ChangeChannelAnnouncementJobTest::shouldHaveDefaultValue()
{
    ChangeChannelAnnouncementJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.announcement().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelAnnouncementJobTest::shouldGenerateRequest()
{
    ChangeChannelAnnouncementJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.setAnnouncement"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeChannelAnnouncementJobTest::shouldGenerateJson()
{
    ChangeChannelAnnouncementJob job;
    const QString roomId = u"foo1"_s;
    const QString announcement = u"topic1"_s;
    job.setRoomId(roomId);
    job.setAnnouncement(announcement);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"announcement":"%1","roomId":"%2"})").arg(announcement, roomId).toLatin1());
}

#include "moc_changechannelannouncementjobtest.cpp"

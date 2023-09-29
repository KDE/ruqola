/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecanceljobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencecanceljob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceCancelJobTest)
using namespace RocketChatRestApi;
VideoConferenceCancelJobTest::VideoConferenceCancelJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceCancelJobTest::shouldHaveDefaultValue()
{
    VideoConferenceCancelJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.callId().isEmpty());
}

void VideoConferenceCancelJobTest::shouldGenerateRequest()
{
    VideoConferenceCancelJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference.cancel")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void VideoConferenceCancelJobTest::shouldGenerateJson()
{
    VideoConferenceCancelJob job;
    job.setCallId(QStringLiteral("foo"));
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"callId":"foo"})").toLatin1());
}

void VideoConferenceCancelJobTest::shouldNotStarting()
{
    VideoConferenceCancelJob job;

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
    job.setCallId(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

#include "moc_videoconferencecanceljobtest.cpp"

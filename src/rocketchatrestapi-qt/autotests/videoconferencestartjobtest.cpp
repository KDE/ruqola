/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencestartjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencestartjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceStartJobTest)
using namespace RocketChatRestApi;
VideoConferenceStartJobTest::VideoConferenceStartJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceStartJobTest::shouldHaveDefaultValue()
{
    VideoConferenceStartJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.info().isValid());
}

void VideoConferenceStartJobTest::shouldGenerateRequest()
{
    VideoConferenceStartJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.start"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void VideoConferenceStartJobTest::shouldGenerateJson()
{
    VideoConferenceStartJob job;
    VideoConferenceStartJob::VideoConferenceStartInfo info;
    info.allowRinging = false;
    info.roomId = "foo"_ba;
    info.title = u"bla"_s;
    job.setInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"allowRinging":false,"roomId":"foo","title":"bla"})").toLatin1());
}

void VideoConferenceStartJobTest::shouldNotStarting()
{
    VideoConferenceStartJob job;

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
    VideoConferenceStartJob::VideoConferenceStartInfo info;
    info.allowRinging = false;
    info.roomId = "foo"_ba;
    info.title = u"bla"_s;
    job.setInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_videoconferencestartjobtest.cpp"

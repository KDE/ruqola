/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecanceljobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencecanceljob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(VideoConferenceCancelJobTest)
using namespace RocketChatRestApi;
VideoConferenceCancelJobTest::VideoConferenceCancelJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceCancelJobTest::shouldHaveDefaultValue()
{
    VideoConferenceCancelJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.callId().isEmpty());
}

void VideoConferenceCancelJobTest::shouldGenerateRequest()
{
    VideoConferenceCancelJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/video-conference.cancel"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void VideoConferenceCancelJobTest::shouldGenerateJson()
{
    VideoConferenceCancelJob job;
    job.setCallId(u"foo"_s);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"callId":"foo"})").toLatin1());
}

void VideoConferenceCancelJobTest::shouldNotStarting()
{
    VideoConferenceCancelJob job;

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
    job.setCallId(u"bla"_s);
    QVERIFY(job.canStart());
}

#include "moc_videoconferencecanceljobtest.cpp"

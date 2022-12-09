/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencejoinjobtest.h"
#include "ruqola_restapi_helper.h"
#include "video-conference/videoconferencejoinjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceJoinJobTest)
using namespace RocketChatRestApi;
VideoConferenceJoinJobTest::VideoConferenceJoinJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceJoinJobTest::shouldHaveDefaultValue()
{
    VideoConferenceJoinJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    // QVERIFY(job.permissions().isEmpty());
}

void VideoConferenceJoinJobTest::shouldGenerateRequest()
{
    VideoConferenceJoinJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/permissions.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void VideoConferenceJoinJobTest::shouldGenerateJson()
{
    VideoConferenceJoinJob job;
#if 0
    QMap<QString, QStringList> lst;
    lst.insert(QStringLiteral("bla"), {QStringLiteral("user"), QStringLiteral("admin")});
    lst.insert(QStringLiteral("team"), {QStringLiteral("user"), QStringLiteral("admin"), QStringLiteral("owner")});
    job.setPermissions(lst);
#endif
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"permissions":[{"_id":"bla","roles":["user","admin"]},{"_id":"team","roles":["user","admin","owner"]}]})").toLatin1());
}

void VideoConferenceJoinJobTest::shouldNotStarting()
{
    VideoConferenceJoinJob job;

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

    // TODO
    QVERIFY(job.canStart());
}

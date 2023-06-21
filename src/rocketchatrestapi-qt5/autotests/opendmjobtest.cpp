/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "opendmjobtest.h"
#include "directmessage/opendmjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(OpenDmJobTest)
using namespace RocketChatRestApi;
OpenDmJobTest::OpenDmJobTest(QObject *parent)
    : QObject(parent)
{
}

void OpenDmJobTest::shouldHaveDefaultValue()
{
    OpenDmJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.directUserId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void OpenDmJobTest::shouldGenerateRequest()
{
    OpenDmJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.open")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void OpenDmJobTest::shouldGenerateJson()
{
    OpenDmJob job;
    const QString channelname = QStringLiteral("foo1");
    job.setDirectUserId(channelname);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(channelname).toLatin1());
}

void OpenDmJobTest::shouldNotStarting()
{
    OpenDmJob job;

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
    const QString username = QStringLiteral("foo1");
    job.setDirectUserId(username);
    QVERIFY(job.canStart());
}

#include "moc_opendmjobtest.cpp"
